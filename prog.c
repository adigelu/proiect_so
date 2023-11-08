#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>

void citire_director(char *director)
{
  DIR *dir;
  dir=opendir(director);
  if(dir==NULL)
    {
      perror("open director");
      exit(1);
    }
  struct dirent *entry;
  char path[300];
  while((entry=readdir(dir))!= NULL)
    {
      sprintf(path,"%s/%s",director,entry->d_name);
      
      printf("%s\n",entry->d_name);
    }
  
  
  if(closedir(dir)==-1)
    {
      perror("close dir");
      exit(1);
    }
  
}

int main(int argc, char *argv[])
{
  char *file=argv[1];
  int fd;
  fd=open(file, O_RDONLY);
  int f=open("statistica.txt",O_WRONLY);
  if(fd==-1){
    perror("error open");
    exit(1);

  }
  int latimea;
  int inaltime;
  lseek(fd,18,SEEK_SET);

  if(read(fd,&latimea,sizeof(int))!=sizeof(int))
    {
      perror("error read");
      exit(1);

    }
  lseek(fd,22,SEEK_SET);
   if(read(fd,&inaltime,sizeof(int))!=sizeof(int))
    {
      perror("error read");
      exit(1);

    }
  
  char buffer[255];
  sprintf(buffer,"nume fisier:%s\n inaltime: %d\n, lungime : %d\n",argv[1],inaltime,latimea);
  // citire_director(argv[2]);
  write(f,buffer,strlen(buffer));
  
  if(close(fd)==-1)
    {
      perror("error close");
    exit(1);
    }
}
