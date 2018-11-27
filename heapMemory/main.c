/* Per thread arena example. */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

// gcc -Wall main.c -o main -lpthread

// cat /proc/424/maps

void *threadFunc(void *arg)
{
  printf("Before malloc in thread 1\n");
  getchar();
  char *addr = (char *)malloc(1000);
  printf("After malloc and before free in thread 1\n");

/*
00400000-00401000 r-xp 00000000 fd:00 3806236                            /home/frank/main
00600000-00601000 r--p 00000000 fd:00 3806236                            /home/frank/main
00601000-00602000 rw-p 00001000 fd:00 3806236                            /home/frank/main
01608000-01629000 rw-p 00000000 00:00 0                                  [heap]
7fe82c000000-7fe82c021000 rw-p 00000000 00:00 0                         // threa arena
7fe82c021000-7fe830000000 ---p 00000000 00:00 0 
7fe8336b5000-7fe8336b6000 ---p 00000000 00:00 0 
7fe8336b6000-7fe833eb6000 rw-p 00000000 00:00 0 
*/
  getchar();
  free(addr);
  printf("After free in thread 1\n");
  getchar();
}

int main()
{
  pthread_t t1;
  void *s;
  int ret;
  char *addr;
  printf("Welcome to per thread arena example::%d\n", getpid());
  printf("Before malloc in main thread\n");

/*
00400000-00401000 r-xp 00000000 fd:00 3806236                            /home/frank/main
00600000-00601000 r--p 00000000 fd:00 3806236                            /home/frank/main
00601000-00602000 rw-p 00001000 fd:00 3806236                            /home/frank/main
01608000-01629000 rw-p 00000000 00:00 0                                  [heap]  // main arena
*/
  getchar();

// 由于132KB比1000字节大很多，所以主线程后续再声请堆空间的话，就会先从这132KB的剩余部分中申请，
// 直到用完或不够用的时候，再通过增加program break location的方式来增加main arena的大小
  addr = (char *)malloc(1000);   
  addr[0] = 'C';
  printf("After malloc and before free in main thread\n");

/*
00400000-00401000 r-xp 00000000 fd:00 3806236                            /home/frank/main
00600000-00601000 r--p 00000000 fd:00 3806236                            /home/frank/main
00601000-00602000 rw-p 00001000 fd:00 3806236                            /home/frank/main
01608000-01629000 rw-p 00000000 00:00 0                                  [heap]  // main arena
*/
  getchar();

  free(addr);
  printf("After free in main thread\n");

  getchar();


  ret = pthread_create(&t1, NULL, threadFunc, NULL);
  if (ret)
  {
    printf("Thread creation error\n");
    return -1;
  }
  ret = pthread_join(t1, &s);
  if (ret)
  {
    printf("Thread join error\n");
    return -1;
  }
  return 0;
}