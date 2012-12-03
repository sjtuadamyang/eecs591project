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
#define MAXBUFFSIZE	1280
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
    char value[1280];
    int i=0;
    int put =0;
    int size=0;
    char attrs[32][32];
    char values[32][1280];
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
                strcpy(attrs[size],p);
                p=strtok(NULL,"@");
                strcpy(values[size],p);
                p=strtok(NULL,"@");
                ++size;
        }
        for(i=0;i<size;++i){
                printf("%d.  attrbute:%s,value:%s\n",i,attrs[i],values[i]);
        }
    
    }
    if(operation[0]=='p')
        printf(":%s\n",value);
    //debug
    hyperclient_returncode retcode;
    hyperclient_returncode loop_status; 
    hyperclient_attribute *attrs;
    size_t attrs_sz = 0;  

    if(operation[0] == 'g')
    {
        // trigger get
        int64_t ret = client->tri_get(keystore, key, strlen(key), handler, strlen(handler), &retcode, &attrs, &attrs_sz);  
        int64_t loop_id = client->loop(-1, &loop_status);
        if(ret != loop_id)
        {
            std::cout<<"error here"<<std::endl;
        }
        else
        {
            buf = (struct data_msgbuf*) malloc(sizeof(struct data_msgbuf));
            if((msgqid = msgget(atoi(id),0666))==-1){
                printf("msgget error. id=%d\n",atoi(id));
                exit(-1);
            }
            //return the message to the webpage frontend
            buf->mtype=1;
            if(retcode != HYPERCLIENT_SUCCESS)
            {
                memcpy(buf->mtext, attrs[0].value, attrs[0].value_sz * sizeof(char));
                memset(buf->mtext+attrs[0].value_sz, '\0', 1);
            }
            else
            {
                std::cout<<"tri get error"<<std::endl;
                memset(buf->mtext+attrs[0].value_sz, '\0', 1);
            }
            msgsnd(msgqid,buf,MAXBUFFSIZE,0);
            free(buf);
        }
    }
    else if(operation[0] == 'p')
    {
        hyperclient_attribute testattr[1];
        testattr[0].attr = "password";
        testattr[0].value = value;
        testattr[0].value_sz = strlen(value);
        testattr[0].datatype = HYPERDATATYPE_STRING;

        //create attrs with attrs_sz
        int64_t ret = client->tri_put(keystore, key, strlen(key), handler, strlen(handler), &retcode, testattr, 1);  
        hyperclient_returncode loop_status;
        int64_t loop_id = client->loop(-1, &loop_status);
        if(ret != loop_id)
        {
            std::cout<<"error here"<<std::endl;
        }
        else
        {
            buf = (struct data_msgbuf*) malloc(sizeof(struct data_msgbuf));
            if((msgqid = msgget(atoi(id),0666))==-1){
                printf("msgget error. id=%d\n",atoi(id));
                exit(-1);
            }
            const char *test = "hello";
            //memset(buf->mtext, 't', 1);
            buf->mtype = 1;
            //memcpy(buf->mtext, test, strlen(test));
            //memset(buf->mtext+strlen(test), '\0', 1);
            strcpy(buf->mtext, test);
            //printf("put4.\n");
            //std::cout<<buf->mtext<<std::endl;
            //printf("test %s", buf->mtext);
            msgsnd(msgqid,buf,MAXBUFFSIZE,0);
            free(buf);
        }
    }
    else if(operation[0]=='s'){
		


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


