#define _GNU_SOURCE
#include <sched.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


// gcc -Wall pid.c -o pid
// 需要root权限执行才能生效

/*
    1: 输出shell的pid 
         echo $$  // 14005

    2：执行程序 ./pid

    3: 输出shell的pid 
         echo $$  // 1
         
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
                         CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD,NULL);
    waitpid(child_fd,NULL,0);
    printf("Main End!\n");
    return 0;
}