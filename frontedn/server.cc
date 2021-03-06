#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<iostream>
#include "thpool.h"
#include "hyperclient.h"
#define DEBUG	1
#define MAXBUFFSIZE	128000
#define THREADPOOL	20
struct id_msgbuf{
    long mtype;
    char mtext[32];
};

struct data_msgbuf{
    long mtype;
    char mtext[MAXBUFFSIZE];
};

void *id_service(void *t){
    int id=30000;
    key_t queue_key=20000;
    int m_type=1;
    struct id_msgbuf *buf;
    int msgqid;
    //create message queue
    if((msgqid=msgget(queue_key,0666|IPC_CREAT))==-1){
        printf("msgget error.\n");
        exit(-1);
    }	
    buf=(struct id_msgbuf*)malloc(sizeof(struct id_msgbuf));
    buf->mtype=m_type;
    while(1)
    {
        sprintf(buf->mtext, "%d", id);
        if(msgsnd(msgqid,buf,32,0)==-1)
            printf("msgsnd error\n");
        if(DEBUG)
            printf("id:%s\n",buf->mtext);
        ++id;
        if(id==40000)
            id=30000;
    }
    pthread_exit(NULL);
}


void task(char* com, void* tmp){
    char* p=0;
    char id[8];
    char operation[2];
    char keystore[32];
    char key[32];
    char handler[32];
    char value[MAXBUFFSIZE];
    int i=0;
    int put =0;
    int size=0;
    char raw_attrs[4][32];
    char raw_values[4][MAXBUFFSIZE];
    hyperclient_attribute put_attrs[32];
    char attr[32];
    char attr_value[32];

    //convert tmp to hyperclient*
    hyperclient *client = (hyperclient *)tmp;

    //for return messages
    int msgqid=0;
    struct data_msgbuf *buf;
    //return messages end

    p=strtok(com,":\";");
    while(p!=NULL){
        //printf("%s\n",p);
        switch(i){
            case 2:strcpy(id,p);break;
            case 3:
                   strcpy(operation,p);		
                   if(operation[0]=='p') put=1;
                   break;
            case 4:	
                   strcpy(keystore,p);
                   break;
            case 5:
                   strcpy(key,p);
                   if(operation[0]=='s') strcpy(attr,p);
                   break;
            case 6:
                   if(put){
                       strcpy(value,p);
                       p=strtok(NULL,":\";");
                   }
                   if(operation[0]=='s') strcpy(attr_value,p);
                   strcpy(handler,p);
                   break;		
        }
        ++i;
        p=strtok(NULL,":\";");		
    }
    printf("%s:%s:%s:%s:%s",id,operation,keystore,key,handler);
    
    if(put){
        //process the values of put operation
        p=strtok(value,"@");
        size=0;
        while(p!=NULL){
            strcpy(raw_attrs[size],p);
            p=strtok(NULL,"@");
            strcpy(raw_values[size],p);
            p=strtok(NULL,"@");
            ++size;
        }
        for(i=0;i<size;++i){
            printf("%d.  attrbute:%s,value:%s\n",i,raw_attrs[i],raw_values[i]);
        }

    }

    //debug
    hyperclient_returncode retcode;
    hyperclient_returncode loop_status; 
    hyperclient_attribute *hyper_attrs;
    size_t hyper_attrs_sz = 0;  
    int64_t ret;
    int64_t loop_id;

    if(operation[0] == 'g')
    {
        int index = 0;  
        if(operation[1] == 't')
        {
            // trigger get
            ret = client->tri_get(keystore, key, strlen(key), handler, strlen(handler), &retcode, &hyper_attrs, &hyper_attrs_sz);  
        }
        else if(operation[1] == 'd')
        {
            //normal get
            ret = client->get(keystore, key, strlen(key), &retcode, &hyper_attrs, &hyper_attrs_sz);  
        }
        loop_id = client->loop(-1, &loop_status);
        if(ret != loop_id)
        {
            std::cout<<"error here"<<std::endl;
        }
        else
        {
            if(operation[1] == 'd')
            {
                for(int j=0; j<hyper_attrs_sz; j++)
                {
                    if(!strcmp(hyper_attrs[j].attr, handler) )
                    {
                        index = j;
                        std::cout<<"we match the handler in the normal get function"<<std::endl;
                    }
                }
            }
            buf = (struct data_msgbuf*) malloc(sizeof(struct data_msgbuf));
            if((msgqid = msgget(atoi(id),0666))==-1){
                printf("msgget error. id=%d\n",atoi(id));
                exit(-1);
            }
            //return the message to the webpage frontend
            buf->mtype=1;
            if(retcode == HYPERCLIENT_SUCCESS)
            {
                memcpy(buf->mtext, hyper_attrs[index].value, hyper_attrs[index].value_sz * sizeof(char));
                memset(buf->mtext+hyper_attrs[index].value_sz, '\0', 1);
                msgsnd(msgqid,buf,hyper_attrs[index].value_sz,0);
            }
            else
            {
                std::cout<<"tri get error"<<std::endl;
                memset(buf->mtext, '\0', 1);
                msgsnd(msgqid,buf,1,0);
            }
            free(buf);
        }
    }
    else if(operation[0] == 'p')
    {
        for(size_t i=0; i<size; i++)
        {
            put_attrs[i].attr = raw_attrs[i];
            put_attrs[i].value = raw_values[i];
            put_attrs[i].value_sz = strlen(raw_values[i]);
            put_attrs[i].datatype = HYPERDATATYPE_STRING;
        }

        //create attrs with attrs_sz
        ret = client->tri_put(keystore, key, strlen(key), handler, strlen(handler), &retcode, put_attrs, size);  
        loop_id = client->loop(-1, &loop_status);
        if(ret != loop_id)
        {
            std::cout<<"error: loop_id and ret is not matched"<<std::endl;
        }
        else
        {
            buf = (struct data_msgbuf*) malloc(sizeof(struct data_msgbuf));
            if((msgqid = msgget(atoi(id),0666))==-1){
                printf("msgget error. id=%d\n",atoi(id));
                exit(-1);
            }
            buf->mtype = 1;
            buf->mtext[0]='e';
            buf->mtext[1]='n';
            buf->mtext[2]='d';
            msgsnd(msgqid,buf,3,0);
            free(buf);
        }
    }
    else if(operation[0]=='s'){
        std::cout<<"search operation: with attr: "<<attr<<" value: "<<attr_value<<std::endl;
        hyperclient_attribute search_attr[1];
        search_attr[0].attr = attr;
        search_attr[0].value = attr_value;
        search_attr[0].value_sz = strlen(attr_value);
        search_attr[0].datatype = HYPERDATATYPE_STRING;
        ret = client->search(keystore, search_attr, 1, NULL, 0, &retcode, &hyper_attrs, &hyper_attrs_sz);

        //use handler to denote the attr that you want for search result
        //we will send a message as value1@value2@value3@value4
        int pos = 0;
        buf = (struct data_msgbuf*) malloc(sizeof(struct data_msgbuf));
        if((msgqid = msgget(atoi(id),0666))==-1){
            printf("msgget error. id=%d\n",atoi(id));
            exit(-1);
        }
        buf->mtype = 1;
        while(1)
        {
            loop_id = client->loop(-1, &loop_status);
            if(loop_id < 0)
            {
                std::cout<<"error: we break because loop_id < 0"<<std::endl;
                break;
            }
            if(retcode == HYPERCLIENT_SEARCHDONE)
            {
                std::cout<<"we break because we finished search"<<std::endl;
                break;
            }
            if(loop_id == ret)
            {
                std::cout<<"we get something, the retcode is "<<loop_status<<std::endl;
                if(pos != 0)
                {
                    //append a @ after pos + hyper_attrs[i].value_sz
                    memset(buf->mtext+pos, '@', 1);
                    pos++;
                }
                //only send the keys back
                memcpy(buf->mtext+pos, hyper_attrs[0].value, hyper_attrs[0].value_sz);
                pos += hyper_attrs[0].value_sz;
            }

        }
        if(pos > MAXBUFFSIZE)
        {
            std::cout<<"error: our message is bigger than MAXBUFFSIZE"<<std::endl;
            msgsnd(msgqid, buf, MAXBUFFSIZE, 0);
        }
        else
        {
            msgsnd(msgqid, buf, pos, 0);
        }
        free(buf);
    }
}

void taskserver(){



	key_t queue_key=20123;
	int m_type=1;
	struct data_msgbuf *buf;
    hyperclient *origin;
	int msgqid;
	int ret_size=0;
	int i=0;
	//init thread pool

	thpool_t* threadpool;
	threadpool=thpool_init(20);
	//create message queue
	if((msgqid=msgget(queue_key,0666|IPC_CREAT))==-1){
		printf("msgget error.\n");
		exit(-1);
	}	
	while(1){
		 //modification 3
                buf=(struct data_msgbuf*)malloc(sizeof(struct data_msgbuf));
                buf->mtype=m_type;
                if((ret_size=msgrcv(msgqid,buf,MAXBUFFSIZE, 1, 0))==-1)
                        printf("msgrcv error.\n");
                printf("Get %s, %d\n", buf->mtext, ret_size);

		thpool_add_work(threadpool,(void* (*)(void *, void *))task, buf->mtext, (void*)NULL);
	}
	thpool_destroy(threadpool);
	pthread_exit(NULL);
}


int main(){
	int rc;
	pthread_t id_thread;
	printf("create the id service thread..\n");
	rc=pthread_create(&id_thread, NULL, id_service, NULL);
	if(rc){
		printf("Error, return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	
	printf("start the get&put service...\n");
	taskserver();
	pthread_exit(NULL);
	return 0;
}


