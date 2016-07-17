#include <stdio.h>
#include <assert.h>

#include "ae.h"

// http://my.oschina.net/u/917596/blog/161077

void loop_init(struct aeEventLoop *l) 
{
        puts("I'm loop_init!!! \n");
}

void file_cb(struct aeEventLoop *l,int fd,void *data,int mask)
{
        char buf[51] ={0};
        read(fd,buf,51);
        printf("I'm file_cb ,here [EventLoop: %p],[fd : %d],[data: %p],[mask: %d] \n",l,fd,data,mask);
        printf("get %s",buf);
}

int time_cb(struct aeEventLoop *l,long long id,void *data)
{
        printf("now is %ld\n",time(NULL));
        printf("I'm time_cb,here [EventLoop: %p],[id : %lld],[data: %p] \n",l,id,data);
        return 5*1000;

}

void fin_cb(struct aeEventLoop *l,void *data)
{
        puts("call the unknow final function \n");
}

int main(int argc,char *argv[])
{
        aeEventLoop *l; 
        char *msg = "Here std say:";
        char *user_data = malloc(50*sizeof(char));
        if(! user_data)
                assert( ("user_data malloc error",user_data) );
        memset(user_data,'\0',50);
        memcpy(user_data,msg,sizeof(msg));

        l = aeCreateEventLoop(1024);
        aeSetBeforeSleepProc(l,loop_init);
        int res;
        //res = aeCreateFileEvent(l,STDIN_FILENO,AE_READABLE,file_cb,user_data);
        //printf("create file event is ok? [%d]\n",res);
        res = aeCreateTimeEvent(l,5*1000,time_cb,NULL,fin_cb);
        printf("create time event is ok? [%d]\n",!res);

        aeMain(l);

        puts("Everything is ok !!!\n");
		return 0;
}

