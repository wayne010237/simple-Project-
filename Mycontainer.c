#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#define SIZE 5000

static char stack[SIZE];



/*
void F1(int ppid){
    pid_t pid;
    printf("ROOT----PID : %d , PARENT PID: %d \n",getpid(),getppid());
    for(int i=0;i<3;i++){
        if((pid=fork())==0){
            printf("child pid: %d, parent pid : %d\n",getpid(),getppid());
            sleep(5);
            return ;
        }
    }
    return ;
}
*/
void grchild(int num)
{
  printf("child(%d) in ns my PID: %d Parent ID=%d\n", num, getpid(),getppid());
  sleep(5);
  puts("end child");
}

int child_fn(int ppid  ) {
  int i;
  printf("PID: %ld Parent:%d\n", (long)getpid(), getppid());
  for(i=0;i<3;i++)
  {
   	if(fork() == 0)
  	{
  		grchild(i+1);  
  		exit(0);
  	}
  	kill(ppid,SIGKILL); // no effect 
  }
  sleep(2);
  kill(2,SIGKILL); // kill the first child
  sleep(10);
  return 0;
  }

int main() {
  pid_t pid = clone(child_fn, stack+5000, CLONE_NEWPID , getpid());
  printf("clone() = %d\n", pid);

  waitpid(pid, NULL, 0);
  return 0;
}
