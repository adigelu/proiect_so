#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

int main()
{
  pid_t pid;
  int status;
  pid=fork();
  if(pid<0)
    {
      perror("eroare pid\n");
      exit(-1);
    }
  
  if(pid==0) //procesul copil
    {
      for(int i=0;i<10;i++)
	{
	  printf("proces copil with pid %d %d\n",getpid(),i);
	}
     
      exit(1);
    }
  else{
    pid=wait(&status);
    printf("proces parinte\n");
    if(WIFEXITED(status))
      {
	printf("child with pid=%d ended with status %d\n",pid,WEXITSTATUS(status));
	
    
      }
  }
}
  
