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
    LOG(INFO)<<"null trigger get called";
    return "";
}
 
