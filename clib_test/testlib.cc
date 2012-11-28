#include <iostream>
#include <string>
#include <glog/logging.h>
#include "hyperclient.h"

extern "C" const std::string 
handler(const char *key, 
        size_t key_sz, 
        hyperclient_attribute *attrs, 
        size_t attrs_sz, 
        hyperclient *messager)
{
    for(size_t i=0; i<attrs_sz; i++)
    {
        LOG(INFO)<<"attr "<<attrs[i].attr<<", value : "<<std::string(attrs[i].value, attrs[i].value_sz);
    }

    return "test reply";
}
 
