#include <iostream>
#include "stdlib.h"
#include <dlfcn.h>

using namespace std;

int main(int argc, char **argv)
{
    void *handle;
    handle = dlopen("/home/adamyang/eecs591project/clib_test/photo.so", RTLD_NOW);
    if(!handle)
    {
        cout<<"error here 1"<<dlerror()<<endl;
        exit(1);
    }

    return 0;
}
