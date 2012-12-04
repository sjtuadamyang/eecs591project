#include "hyperclient/hyperclient.h"
#include <iostream>

using namespace std;

int main()
{
    cout<<"test start "<<endl;

    //first we put a entry into the keystore
    hyperclient_attribute test_attr[3];

    test_attr[0].attr = "phone";
    test_attr[0].value = "123455";
    test_attr[0].value_sz = strlen(test_attr[0].value);
    test_attr[0].datatype = HYPERDATATYPE_STRING;

    test_attr[1].attr = "last";
    test_attr[1].value = "Yang";
    test_attr[1].value_sz = strlen(test_attr[1].value);
    test_attr[1].datatype = HYPERDATATYPE_STRING;

    test_attr[2].attr = "first";
    test_attr[2].value = "Fred";
    test_attr[2].value_sz = strlen(test_attr[2].value);
    test_attr[2].datatype = HYPERDATATYPE_STRING;

    hyperclient_returncode retcode;
    const char *key = "adamyang";
    hyperclient test_client("127.0.0.1", 1234);
    const char *trigger = "testtrigger";

    int64_t ret = test_client.tri_put("phonebook", key, strlen(key), trigger, strlen(trigger), &retcode, test_attr, 3);

    hyperclient_returncode loop_status;
    int64_t loop_id = test_client.loop(-1, &loop_status);

    if(ret != loop_id)
    {
        cout <<"exit here 1"<<endl;
        exit(1);
    }

    cout<<"put ret is "<<ret<<" return code is "<<retcode<<endl;

    //define the container for the attribute get
    //we then use tri_get to get the data out, which will returned triggered output
    const char *key2 = "andywang";

    hyperclient_attribute *test_get_attr;
    size_t get_size = 0;
    ret = test_client.tri_get("phonebook", key2, strlen(key2), trigger, strlen(trigger), &retcode, &test_get_attr, &get_size);
    
    loop_id = test_client.loop(-1, &loop_status);

    if(ret != loop_id)
    {
        cout <<"exit here 2"<<endl;
        exit(1);
    }

    if(get_size!=0)
    {
        cout<<"we get something "<<endl;
        for(int i=0; i<get_size; i++)
        {
            cout<<"attr "<<i<<": "<<test_get_attr[i].attr<<", value : "<<string(test_get_attr[i].value, test_get_attr[i].value_sz)<<endl;
        }
    }
    else
    {
        cout<<"we get nothing"<<endl;
    }

    cout<<"get ret is "<<ret<<" return code is "<<retcode<<endl;

    //This part will be used to test the search function
    hyperclient_attribute search_attr[1];
    search_attr[0].attr = "first";
    search_attr[0].value = "adam";
    search_attr[0].value_sz = strlen(search_attr[0].value);
    search_attr[0].datatype = HYPERDATATYPE_STRING;
    ret = test_client.search("phonebook", search_attr, 1, NULL, 0, &retcode, &test_get_attr, &get_size);

    while(1)
    {
        loop_id = test_client.loop(-1, &loop_status);
        if(loop_id < 0)
        {
            cout<<"we break because loop_id < 0"<<endl;
            break;
        }
        if(retcode == HYPERCLIENT_SEARCHDONE)
        {
            cout<<"we break because we finished search"<<endl;
            break;
        }
        if(loop_id == ret)
        {
            cout<<"we get something, the retcode is "<<loop_status<<endl;
            for(int i=0; i<get_size; i++)
            {
                cout<<"search out: attr "<<i<<": "<<test_get_attr[i].attr<<", value : "<<string(test_get_attr[i].value, test_get_attr[i].value_sz)<<endl;
            }
        }
    }

    if(loop_status != HYPERCLIENT_SEARCHDONE)
        cout<<"search error happens loop_status is "<<loop_status<<endl;

    
    return 0;
}
