#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <glog/logging.h>
#include <Magick++.h>
#include "base64.h"
#include "hyperclient.h"

// what this trigger do, this will execute a very dummy operation:
// like dummy loop with very dummy operation, and return a random image
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

    std::vector<std::string> results;

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

            std::string str (hyper_attr[0].value, hyper_attr[0].value_sz);

            results.push_back(str);

            LOG(INFO)<<"current result "<<result;
        }
    }
    //dummy operation
    int waste;
    for(int i=0; i < 10000; i++) {
        waste++;
    }

    hyperclient_attribute *hyper_get_attr;
    size_t get_size = 0;

    srand ( time(NULL) );

    int id = rand() % results.size();
    //get content
    ret = message->get("photo_l", results[id].c_str(), strlen(results[id].c_str()), &retcode, &hyper_get_attr, &get_size);

    loop_id = message->loop(-1, &loop_status);

    if(ret != loop_id)
    {
        LOG(INFO)<<"some ERROR here";
        exit(1);
    }

    if(get_size!=0)
    {
        LOG(INFO)<<"we get something";

        return string(hyper_get_attr[0].value, hyper_get_attr[0].value_sz);

    }
    else
    {
        LOG(INFO)<<"we get Nothing, get size is 0";
    }

    //build string fill with the content
    std::string dummy = "nothing";

    return dummy;
}
