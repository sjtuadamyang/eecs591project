#include <iostream>
#include <string>
#include <glog/logging.h>
#include <Magick++.h>
#include "base64.h"
#include "hyperclient.h"

extern "C" const std::string
handler(const char *key,
        size_t key_sz,
        hyperclient_attribute *attrs,
        size_t attrs_sz,
        hyperclient *message)
{
    std::string result;
    //use the key as the username of the space "photoinfo" and do search
    hyperclient_attribute search_attr[1];
    search_attr[0].attr = "username";
    search_attr[0].value = key;
    search_attr[0].value_sz = key_sz;
    search_attr[0].datatype = HYPERDATATYPE_STRING;

    hyperclient_returncode retcode;
    hyperclient_returncode loop_status;
    hyperclient_attribute *hyper_attr;
    size_t hyper_attr_sz = 0;

    int64_t loop_id;
    int64_t ret = message->search("photoinfo", search_attr, 1, NULL, 0, &retcode, &hyper_attr, &hyper_attr_sz);
    bool pos = true;

    while(1)
    {
        loop_id = message->loop(-1, &loop_status);
        if(loop_id < 0)
        {
            LOG(INFO)<<"we break because loop_id < 0";
            break;
        }
        if(retcode == HYPERCLIENT_SEARCHDONE)
        {
            LOG(INFO)<<"we break because we finished search";
            break;
        }
        if(loop_id == ret)
        {
            LOG(INFO)<<"we get something, format the result";
            if(!pos)
            {
                result.append(1, '@');
            }
            pos = false;
            result.append(hyper_attr[0].value, hyper_attr[0].value_sz);
            LOG(INFO)<<"current result "<<result;
        }
    }

    return result;
}
