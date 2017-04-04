#define _GNU_SOURCE
#include <sched.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


// gcc -Wall utc.c -o utc 
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
                        CLONE_NEWUTS | SIGCHLD,NULL);
    waitpid(child_fd,NULL,0);
    printf("Main End!\n");
    return 0;
}