#define _GNU_SOURCE
#include <sched.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


// gcc -Wall ipc.c -o ipc 

/*
  1：创建message queue
  ipcmk -Q
    输出： 消息队列 id：0

  2：查看已经开启的message queue
  ipcs -q
    输出：
        --------- 消息队列 -----------
        键        msqid      拥有者  权限     已用字节数 消息
        0x16a93c26 0          frank      644        0            0
 
  3: 执行ipc程序，然后执行ipcs -q，我们发现之前的message queue不见了

  4：exit 退出

  5：ipcs -q,查看已经开启的message queue
 
*/
#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
    "/bin/bash",
    NULL
};

int child_main(void* args){
    printf("Child Process！");
    sethostname("NewNamespace",12);
    execv(child_args[0],child_args);
    return 1;
}

int main(){
    printf("Main Start!\n");
    int child_fd = clone(child_main,child_stack+STACK_SIZE,
                       CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD,NULL);
    waitpid(child_fd,NULL,0);
    printf("Main End!\n");
    return 0;
}