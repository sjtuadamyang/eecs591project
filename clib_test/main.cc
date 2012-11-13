#include <iostream>
#include "stdlib.h"
#include <dlfcn.h>

using namespace std;

int main(int argc, char **argv)
{
    void *handle;
    handle = dlopen("/home/adamyang/eecs591project/clib_test/testlib.so", RTLD_NOW);
    if(!handle)
    {
        cout<<"error here 1"<<endl;
        exit(1);
    }
    int (*test)();
    test = (int (*)())dlsym(handle, "test_func");
    if(!test)
    {
        cout<<"error here 2"<<endl;
        exit(1);
    }
    test();

    return 0;
}
