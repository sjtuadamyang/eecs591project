// Copyright (c) 2011, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// POSIX
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>

// Google Log
#include <glog/logging.h>

// po6
#include <po6/net/location.h>

// HyperDisk
#include "hyperdisk/hyperdisk/reference.h"

// HyperDex
#include "hyperdex/hyperdex/microop.h"
#include "hyperdex/hyperdex/network_constants.h"
#include "hyperdex/hyperdex/packing.h"

// HyperDaemon
#include "hyperdaemon/datalayer.h"
#include "hyperdaemon/logical.h"
#include "hyperdaemon/network_worker.h"
#include "hyperdaemon/ongoing_state_transfers.h"
#include "hyperdaemon/replication_manager.h"
#include "hyperdaemon/searches.h"

// Dynamic lib
#include <dlfcn.h>

// hyperclient
#include "hyperclient/hyperclient.h"

using hyperdex::entityid;
using hyperdex::network_msgtype;
using hyperdex::network_returncode;

hyperdaemon :: network_worker :: network_worker(datalayer* data,
                                                logical* comm,
                                                hyperclient* messenger,
                                                searches* ssss,
                                                ongoing_state_transfers* ost,
                                                replication_manager* repl)
    : m_continue(true)
    , m_data(data)
    , m_comm(comm)
    , m_messenger(messenger)
    , m_ssss(ssss)
    , m_ost(ost)
    , m_repl(repl)
{
}

hyperdaemon :: network_worker :: ~network_worker() throw ()
{
    if (m_continue)
    {
        m_continue = false;
        LOG(INFO) << "Network worker object not cleanly shutdown.";
    }
}

void
hyperdaemon :: network_worker :: run()
{
    sigset_t ss;

    if (sigfillset(&ss) < 0)
    {
        PLOG(ERROR) << "sigfillset";
        return;
    }

    if (pthread_sigmask(SIG_BLOCK, &ss, NULL) < 0)
    {
        PLOG(ERROR) << "pthread_sigmask";
        return;
    }

    entityid from;
    entityid to;
    network_msgtype type;
    std::auto_ptr<e::buffer> msg;
    unsigned int seed = pthread_self();

    while (m_continue && m_comm->recv(&from, &to, &type, &msg))
    {
        e::buffer::unpacker up = msg->unpack_from(m_comm->header_size());
        uint64_t nonce;

        if (type == hyperdex::REQ_GET)
        {
            e::slice key;

            if ((up >> nonce >> key).error())
            {
                LOG(WARNING) << "unpack of REQ_GET failed; here's some hex:  " << msg->hex();
                continue;
            }

            std::vector<e::slice> value;
            uint64_t version;
            hyperdisk::reference ref;
            network_returncode result;

            switch (m_data->get(to.get_region(), key, &value, &version, &ref))
            {
                case hyperdisk::SUCCESS:
                    result = hyperdex::NET_SUCCESS;
                    break;
                case hyperdisk::NOTFOUND:
                    result = hyperdex::NET_NOTFOUND;
                    break;
                case hyperdisk::WRONGARITY:
                    result = hyperdex::NET_BADDIMSPEC;
                    break;
                case hyperdisk::MISSINGDISK:
                    LOG(ERROR) << "GET caused a MISSINGDISK at the data layer.";
                    result = hyperdex::NET_SERVERERROR;
                    break;
                case hyperdisk::DATAFULL:
                case hyperdisk::SEARCHFULL:
                case hyperdisk::SYNCFAILED:
                case hyperdisk::DROPFAILED:
                case hyperdisk::SPLITFAILED:
                case hyperdisk::DIDNOTHING:
                default:
                    LOG(ERROR) << "GET returned unacceptable error code.";
                    result = hyperdex::NET_SERVERERROR;
                    break;
            }

            size_t sz = m_comm->header_size() + sizeof(uint64_t)
                      + sizeof(uint16_t) + hyperdex::packspace(value);
            msg.reset(e::buffer::create(sz));
            e::buffer::packer pa = msg->pack_at(m_comm->header_size());
            pa = pa << nonce << static_cast<uint16_t>(result) << value;
            assert(!pa.error());
            m_comm->send(to, from, hyperdex::RESP_GET, msg);
        }
        else if (type == hyperdex::REQ_PUT)
        {
            uint32_t attrs_sz;
            e::slice key;
            std::vector<std::pair<uint16_t, e::slice> > attrs;
            up = up >> nonce >> key >> attrs_sz;

            for (uint32_t i = 0; i < attrs_sz; ++i)
            {
                uint16_t dimnum;
                e::slice val;
                up = up >> dimnum >> val;
                attrs.push_back(std::make_pair(dimnum, val));
            }

            if (up.error())
            {
                LOG(WARNING) << "unpack of REQ_PUT failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->client_put(from, to, nonce, msg, key, attrs);
        }
        else if (type == hyperdex::REQ_TRIGET)
	    {
            e::slice key;
            e::slice trigger;

            if ((up >> nonce >> key >> trigger).error())
            {
                LOG(WARNING) << "unpack of REQ_TRIGET failed; here's some hex:  " << msg->hex();
                continue;
            }

            std::vector<e::slice> value;
            std::vector<e::slice> real_value;
            uint64_t version;
            hyperdisk::reference ref;
            network_returncode result;

            //keep this part, we need to get the value any way
            switch (m_data->get(to.get_region(), key, &value, &version, &ref))
            {
                case hyperdisk::SUCCESS:
                    result = hyperdex::NET_SUCCESS;
                    break;
                case hyperdisk::NOTFOUND:
                    result = hyperdex::NET_NOTFOUND;
                    break;
                case hyperdisk::WRONGARITY:
                    result = hyperdex::NET_BADDIMSPEC;
                    break;
                case hyperdisk::MISSINGDISK:
                    LOG(ERROR) << "GET caused a MISSINGDISK at the data layer.";
                    result = hyperdex::NET_SERVERERROR;
                    break;
                case hyperdisk::DATAFULL:
                case hyperdisk::SEARCHFULL:
                case hyperdisk::SYNCFAILED:
                case hyperdisk::DROPFAILED:
                case hyperdisk::SPLITFAILED:
                case hyperdisk::DIDNOTHING:
                default:
                    LOG(ERROR) << "GET returned unacceptable error code.";
                    result = hyperdex::NET_SERVERERROR;
                    break;
            }

            // here we will convert value to hyperclient_attribute[] with size and pass it to handler function
            LOG(INFO) << "test value size is "<<value.size();
            std::vector<hyperclient_attribute> ha;  
            size_t size = value.size() * sizeof(hyperclient_attribute);
            hyperclient_returncode retcode;
            std::vector<hyperdex::attribute> dimension_names = (m_messenger->get_config(&retcode))->dimension_names(to.get_space());
            
            for (size_t i = 0; i < value.size(); ++i)
            {
                size += dimension_names[i+1].name.size() + 1 + value[i].size();
            }
            char *ret = static_cast<char *>(malloc(size));
            char *data = ret + sizeof(hyperclient_attribute) * value.size();

            LOG(INFO) << "dimension size is "<<dimension_names.size();
            for(int i=0; i<dimension_names.size(); i++)
            {
                LOG(INFO) << "test name "<< i << dimension_names[i].name;
            }

            for (size_t i = 0; i < value.size(); ++i)
            {
                ha.push_back(hyperclient_attribute());
                size_t attr_sz = dimension_names[i + 1].name.size() + 1;
                ha.back().attr = data;
                memmove(data, dimension_names[i + 1].name.c_str(), attr_sz);
                data += attr_sz;
                ha.back().value = data;
                memmove(data, value[i].data(), value[i].size());
                data += value[i].size();
                ha.back().value_sz = value[i].size();
                ha.back().datatype = dimension_names[i + 1].type;
            }

            memmove(ret, &ha.front(), sizeof(hyperclient_attribute) * ha.size());

            hyperclient_attribute *attrs = reinterpret_cast<hyperclient_attribute*>(ret);
            size_t attrs_sz = ha.size();
            for(size_t i = 0; i < attrs_sz; i++)
            {
                LOG(INFO) << "test attr " << i << "name: " << attrs[i].attr << " value: " << std::string(attrs[i].value, attrs[i].value_sz);
            }

            //trigger handler goes here
            //prototype will be const string& handler(const char *key, size_t key_sz, hyperclient_attribute *attrs, size_t attrs_sz, hyperclient *messager)
            //real_value will be ret.c_str()
            std::string base_addr = std::string("/home/adamyang/eecs591project/clib_test/.so");
            size_t posl = base_addr.size() - 3;
            base_addr.insert(posl, reinterpret_cast<const char*>(trigger.data()), trigger.size());

            LOG(INFO) <<"check addr: " << base_addr;

            //load trigger function and execute it
            void *handle;
            handle = dlopen(base_addr.c_str(), RTLD_NOW);
            std::string handle_value;

            if(handle)
            {
                LOG(INFO) << "open so file success";
                const std::string (*test)(const char*, size_t, hyperclient_attribute*, size_t, hyperclient*) = (const std::string (*)(const char*, size_t, hyperclient_attribute*, size_t, hyperclient*))dlsym(handle, "handler");
                handle_value = test(reinterpret_cast<const char*>(key.data()), key.size(), attrs, attrs_sz, m_messenger);
                real_value.push_back(e::slice(handle_value));
                dlclose(handle);
            }

            //this free will make all the hyperclient_attribute destroyed
            free(ret);

            // normal get routine, should return the real_value to the real client
            size_t sz = m_comm->header_size() + sizeof(uint64_t)
                      + sizeof(uint16_t) + hyperdex::packspace(real_value);
            msg.reset(e::buffer::create(sz));
            e::buffer::packer pa = msg->pack_at(m_comm->header_size());
            pa = pa << nonce << static_cast<uint16_t>(result) << real_value;
            assert(!pa.error());
            m_comm->send(to, from, hyperdex::RESP_TRIGET, msg);

            LOG(INFO) << "test of req trigger get success.";
	    }
        else if (type == hyperdex::REQ_TRIPUT)
	    {
            uint32_t attrs_sz;
            e::slice key;
            e::slice trigger;
            std::vector<std::pair<uint16_t, e::slice> > attrs;
            up = up >> nonce >> key >> trigger >> attrs_sz;

            for (uint32_t i = 0; i < attrs_sz; ++i)
            {
                uint16_t dimnum;
                e::slice val;
                up = up >> dimnum >> val;
                attrs.push_back(std::make_pair(dimnum, val));
            }

            if (up.error())
            {
                LOG(WARNING) << "unpack of REQ_PUT failed; here's some hex:  " << msg->hex();
                continue;
            }

            // here we will convert attrs to hyperclient_attribute[] with size and pass it to handler function
            LOG(INFO) << "test value attrs is "<<attrs.size();
            std::vector<hyperclient_attribute> ha;  
            size_t size = attrs.size() * sizeof(hyperclient_attribute);
            hyperclient_returncode retcode;
            std::vector<hyperdex::attribute> dimension_names = (m_messenger->get_config(&retcode))->dimension_names(to.get_space());
            
            for (size_t i=0; i<attrs.size(); ++i)
            {
                LOG(INFO) << "dimen is " << attrs[i].first <<" value is "<<std::string(reinterpret_cast<const char *>(attrs[i].second.data()), attrs[i].second.size());
            }

            for (size_t i = 0; i < attrs.size(); ++i)
            {
                size += dimension_names[attrs[i].first].name.size() + 1 + attrs[i].second.size();
            }
            char *ret = static_cast<char *>(malloc(size));
            char *data = ret + sizeof(hyperclient_attribute) * attrs.size();

            LOG(INFO) << "dimension size is "<<dimension_names.size();
            for(int i=0; i<dimension_names.size(); i++)
            {
                LOG(INFO) << "test name "<< i << dimension_names[i].name;
            }

            for (size_t i = 0; i < attrs.size(); ++i)
            {
                ha.push_back(hyperclient_attribute());
                size_t attr_sz = dimension_names[attrs[i].first].name.size() + 1;
                ha.back().attr = data;
                memmove(data, dimension_names[attrs[i].first].name.c_str(), attr_sz);
                data += attr_sz;
                ha.back().value = data;
                memmove(data, attrs[i].second.data(), attrs[i].second.size());
                data += attrs[i].second.size();
                ha.back().value_sz = attrs[i].second.size();
                ha.back().datatype = dimension_names[attrs[i].first].type;
            }

            memmove(ret, &ha.front(), sizeof(hyperclient_attribute) * ha.size());

            hyperclient_attribute *hyper_attrs = reinterpret_cast<hyperclient_attribute*>(ret);
            size_t hyper_attrs_sz = ha.size();
            for(size_t i = 0; i < hyper_attrs_sz; i++)
            {
                LOG(INFO) << "test attr " << i << "name: " << hyper_attrs[i].attr << " value: " << std::string(hyper_attrs[i].value, hyper_attrs[i].value_sz);
            }

            //trigger handler goes here
            //prototype will be 
            //  const std::string 
            //  handler(const char *key, 
            //          size_t key_sz, 
            //          hyperclient_attribute *attrs, 
            //          size_t attrs_sz, 
            //          hyperclient *messager)

            std::string base_addr = std::string("/home/adamyang/eecs591project/clib_test/.so");
            size_t posl = base_addr.size() - 3;
            base_addr.insert(posl, reinterpret_cast<const char*>(trigger.data()), trigger.size());

            LOG(INFO) <<"check addr: " << base_addr;

            //load trigger function and execute it
            void *handle;
            handle = dlopen(base_addr.c_str(), RTLD_NOW);
            std::string handle_value;

            if(handle)
            {
                LOG(INFO) << "open so file success";
                const std::string (*test)(const char*, size_t, hyperclient_attribute*, size_t, hyperclient*) = (const std::string (*)(const char*, size_t, hyperclient_attribute*, size_t, hyperclient*))dlsym(handle, "handler");
                handle_value = test(reinterpret_cast<const char*>(key.data()), key.size(), hyper_attrs, hyper_attrs_sz, m_messenger);
                //real_value.push_back(e::slice(handle_value));
                dlclose(handle);
            }

            //this free will make all the hyperclient_attribute destroyed
            free(ret);

            m_repl->client_put(from, to, nonce, msg, key, attrs);
            LOG(INFO) << "test of req trigger put success.";
	    }
        else if (type == hyperdex::REQ_CONDPUT)
        {
            uint32_t cond_sz;
            uint32_t attrs_sz;
            e::slice key;
            std::vector<std::pair<uint16_t, e::slice> > condattrs;
            std::vector<std::pair<uint16_t, e::slice> > attrs;
            up = up >> nonce >> key >> cond_sz;

            for (uint32_t i = 0; i < cond_sz; ++i)
            {
                uint16_t dimnum;
                e::slice val;
                up = up >> dimnum >> val;
                condattrs.push_back(std::make_pair(dimnum, val));
            }

            up = up >> attrs_sz;

            for (uint32_t i = 0; i < attrs_sz; ++i)
            {
                uint16_t dimnum;
                e::slice val;
                up = up >> dimnum >> val;
                attrs.push_back(std::make_pair(dimnum, val));
            }

            if (up.error())
            {
                LOG(WARNING) << "unpack of REQ_CONDPUT failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->client_condput(from, to, nonce, msg, key, condattrs, attrs);
        }
        else if (type == hyperdex::REQ_DEL)
        {
            e::slice key;
            up = up >> nonce >> key;

            if (up.error())
            {
                LOG(WARNING) << "unpack of REQ_DEL failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->client_del(from, to, nonce, msg, key);
        }
        else if (type == hyperdex::REQ_ATOMIC)
        {
            uint32_t num_microops;
            e::slice key;
            std::vector<hyperdex::microop> microops;
            up = up >> nonce >> key >> num_microops;
            microops.reserve(num_microops);

            for (uint32_t i = 0; i < num_microops; ++i)
            {
                hyperdex::microop o;
                up = up >> o;
                microops.push_back(o);
            }

            if (up.error())
            {
                LOG(WARNING) << "unpack of REQ_ATOMIC failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->client_atomic(from, to, nonce, msg, key, &microops);
        }
        else if (type == hyperdex::REQ_SEARCH_START)
        {
            uint64_t searchid;
            hyperspacehashing::search s(0);

            if ((up >> nonce >> searchid >> s).error())
            {
                LOG(WARNING) << "unpack of REQ_SEARCH_START failed; here's some hex:  " << msg->hex();
                continue;
            }

            if (s.sanity_check())
            {
                m_ssss->start(to, from, searchid, nonce, msg, s);
            }
            else
            {
                LOG(INFO) << "Dropping search which fails sanity_check.";
            }
        }
        else if (type == hyperdex::REQ_SEARCH_NEXT)
        {
            uint64_t searchid;

            if ((up >> nonce >> searchid).error())
            {
                LOG(WARNING) << "unpack of REQ_SEARCH_NEXT failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_ssss->next(to, from, searchid, nonce);
        }
        else if (type == hyperdex::REQ_SEARCH_STOP)
        {
            uint64_t searchid;

            if ((up >> nonce >> searchid).error())
            {
                LOG(WARNING) << "unpack of REQ_SEARCH_STOP failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_ssss->stop(to, from, searchid);
        }
        else if (type == hyperdex::CHAIN_PUT)
        {
            uint64_t version;
            uint8_t fresh;
            e::slice key;
            std::vector<e::slice> value;

            if ((up >> version >> fresh >> key >> value).error())
            {
                LOG(WARNING) << "unpack of CHAIN_PUT failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->chain_put(from, to, version, fresh == 1, msg, key, value);
        }
        else if (type == hyperdex::CHAIN_DEL)
        {
            uint64_t version;
            e::slice key;

            if ((up >> version >> key).error())
            {
                LOG(WARNING) << "unpack of CHAIN_DEL failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->chain_del(from, to, version, msg, key);
        }
        else if (type == hyperdex::CHAIN_SUBSPACE)
        {
            uint64_t version;
            e::slice key;
            std::vector<e::slice> value;
            uint64_t nextpoint;

            if ((up >> version >> key >> value >> nextpoint).error())
            {
                LOG(WARNING) << "unpack of CHAIN_SUBSPACE failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->chain_subspace(from, to, version, msg, key, value, nextpoint);
        }
        else if (type == hyperdex::CHAIN_ACK)
        {
            uint64_t version;
            e::slice key;

            if ((up >> version >> key).error())
            {
                LOG(WARNING) << "unpack of CHAIN_ACK failed; here's some hex:  " << msg->hex();
                continue;
            }

            m_repl->chain_ack(from, to, version, msg, key);
        }
        else if (type == hyperdex::XFER_MORE)
        {
            m_ost->region_transfer_send(from, to);
        }
        else if (type == hyperdex::XFER_DONE)
        {
            m_ost->region_transfer_done(from, to);
        }
        else if (type == hyperdex::XFER_DATA)
        {
            uint64_t xfer_num;
            uint8_t op;
            uint64_t version;
            e::slice key;
            std::vector<e::slice> value;

            if ((up = up >> xfer_num >> version >> key >> op).error())
            {
                LOG(WARNING) << "unpack of XFER_DATA failed; here's some hex:  " << msg->hex();
                continue;
            }

            if (op)
            {
                if ((up >> value).error())
                {
                    LOG(WARNING) << "unpack of XFER_DATA failed; here's some hex:  " << msg->hex();
                    continue;
                }
            }

            m_ost->region_transfer_recv(from, to.subspace, xfer_num,
                                        op == 1, version, msg, key, value);
        }
        else
        {
            LOG(INFO) << "Message of unknown type received.";
        }

        if (rand_r(&seed) < (0.01 * RAND_MAX))
        {
            m_data->flush(to.get_region(), 100000, true);
        }
    }
}

void
hyperdaemon :: network_worker :: shutdown()
{
    // TODO:  This is not the proper shutdown method.  Proper shutdown is a
    // two-stage process, and requires global coordination.
    m_continue = false;
}
