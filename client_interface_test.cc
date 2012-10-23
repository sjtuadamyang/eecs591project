#include "hyperclient/hyperclient.h"
#include <iostream>

using namespace std;

int main()
{
    //define hyper attribute
    hyperclient_attribute test_attr[3];

    test_attr[0].attr = "first";
    test_attr[0].value = "Fred";
    test_attr[0].value_sz = strlen(test_attr[0].value);
    test_attr[0].datatype = HYPERDATATYPE_STRING;

    test_attr[1].attr = "last";
    test_attr[1].value = "Yang";
    test_attr[1].value_sz = strlen(test_attr[1].value);
    test_attr[1].datatype = HYPERDATATYPE_STRING;

    test_attr[2].attr = "phone";
    test_attr[2].value = "123455";
    test_attr[2].value_sz = strlen(test_attr[2].value);
    test_attr[2].datatype = HYPERDATATYPE_INT64;

    hyperclient_returncode retcode;
    const char *key = "adamyang";
    hyperclient test_client("127.0.0.1", 1234);
    int64_t ret = test_client.put("phonebook", key, strlen(key), test_attr, 3, &retcode);

    //define the container for the attribute get
    hyperclient_attribute *test_get_attr;
    size_t get_size = 0;
    ret = test_client.get("phonebook", key, strlen(key), &retcode, &test_get_attr, &get_size);
    if(get_size!=0)
    {
        cout<<"we get something "<<endl;
        for(int i=0; i<get_size; i++)
        {
            cout<<"attr "<<i<<": "<<test_get_attr[i].attr<<", "<<test_get_attr[i].value<<endl;
        }
    }
    else
    {
        cout<<"we get nothing"<<endl;
    }

    cout<<"ret is "<<ret<<" return code is "<<retcode<<endl;

    return 0;
}
