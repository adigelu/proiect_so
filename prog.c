#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<ctype.h>

void executa(char *path,char *nume,struct stat *st_file)
{
  char buffer[5000]="";
  char a[50]="";
  strcpy(buffer,"nume:");
  strcat(buffer,nume);
  strcat(buffer,"\n");
  printf("in");
  if(S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")==0)
  {
    char *file=path;
    int fd;
  fd=open(file, O_RDONLY);
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
    sprintf(a,"inaltime:%d \n latime:%d\n",inaltime,latimea);
    //strcat(buffer,inaltime);
    //strcat(buffer,"\n");
    //strcat(buffer,latimea);
    strcat(buffer,a);
    if(close(fd)==-1)
	{
	  perror("error close");
	  exit(1);
	}
  }
  if(S_ISLNK(st_file->st_mode))
  {
    struct stat lst_file;
    if(lstat(path,&lst_file) == -1)
    {
      perror("stat error");
      exit(1);
    }
    
    sprintf(a, "dime: %ld\n", lst_file.st_size);
    strcat(buffer,a);
    strcat(buffer,"\n");
  }
  if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")==0) || S_ISDIR(st_file->st_mode))
  {
    strcat(buffer,"utilizator:");
    sprintf(a,"%d\n", st_file->st_uid);
    strcat(buffer,a);
    
  }
  if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")==0) || S_ISLNK(st_file->st_mode))
  {
    strcat(buffer,"dimensiune:");
    sprintf(a,"%ld\n", st_file->st_size);
    strcat(buffer,a);
  }
   if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")==0))
   {
    strcat(a,"ultima modificare:");
    sprintf(a,"%s", ctime(&st_file->st_mtime));
    strcat(buffer,a);
    strcat(buffer,"contor de legaturi:");
    sprintf(a, "%ld\n", st_file->st_nlink);
    strcat(buffer,a);
   }
   struct stat dim=*st_file;
   char acces_usser[4]="";
  if(dim.st_mode & S_IRWXU)
  {
    acces_usser[0]='R';
  }
  else{
    acces_usser[0]='-';
  }
  if(dim.st_mode & S_IWUSR)
  {
    acces_usser[1]='W';
  }
   else{
    acces_usser[1]='-';
  }
  if(dim.st_mode & S_IXUSR)
  {
    acces_usser[2]='X';
  }
   else{
    acces_usser[2]='-';
  }
  char access_grup[4]="";
  if(dim.st_mode & S_IRGRP)
  {
    access_grup[0]='R';
  }
  else{
    access_grup[0]='-';
  }
  if(dim.st_mode & S_IWGRP )
  {
    access_grup[1]='W';
  }
  else{
    access_grup[1]='-';
  }
  if(dim.st_mode & S_IXGRP)
  {
    access_grup[2]='X';
  }
  else{
    access_grup[2]='-';
  }
    char access_altii[4]="";
     if(dim.st_mode & S_IROTH)
  {
    access_altii[0]='R';
  }
  else{
    access_altii[0]='-';
  }
  if(dim.st_mode & S_IWOTH  )
  {
    access_altii[1]='W';
  }
  else{
    access_altii[1]='-';
  }
  if(dim.st_mode & S_IROTH)
  {
    access_altii[2]='X';
  }
  else{
    access_altii[2]='-';
  }
  strcat(buffer,"drepturi de acces user: ");
  strcat(buffer,acces_usser);
  strcat(buffer,"\n");
  strcat(buffer,"drepturi de acces grup: ");
  strcat(buffer,access_grup);
  strcat(buffer,"\n");
  strcat(buffer,"drepturi de acces altii: ");
  strcat(buffer,access_altii);
  strcat(buffer,"\n");
int f=open("statistica.txt",O_WRONLY | O_APPEND);
if(write(f,buffer,strlen(buffer))==-1)
{
  perror("eroare scriere");
};
//write(f,acces_usser,strlen(acces_usser));
//write(f,access_grup,strlen(access_grup));
//write(f,access_altii,strlen(access_altii));
close(f);
//close(fd);
}
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
      //printf("while\n");
    
    
    if(strcmp(entry->d_name,"..")!=0 && strcmp(entry->d_name,".")!=0)
    {
      struct stat st_file;
      //printf("str\n");
        if(stat(path,&st_file) == -1)
            {
            perror("in stat error");
            exit(1);
            }
        if(S_ISDIR(st_file.st_mode))
        {
          executa(path,entry->d_name,&st_file);
          citire_director(path);
        }
        else{
          //printf("else\n");
          executa(path,entry->d_name,&st_file);
        }
    }
    }
  
  
  if(closedir(dir)==-1)
    {
      perror("close dir");
      exit(1);
    }
  
}

int main(int argc, char *argv[])
{
  //char *file=argv[1];
  /*int fd;
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
   int dimensiune;
   struct stat dim;
   if(stat("imagine_1.bmp",&dim)==-1)
     {
       perror("stat");
       exit(1);
     }
   
   dimensiune=dim.st_size;
   int user=dim.st_uid;
   //time_t timp =*ctime(dim.st_mtim);
   int nr_leg=dim.st_nlink;
  char acces_usser[3]="";
  if(dim.st_mode & S_IRWXU)
  {
    acces_usser[0]='R';
  }
  else{
    acces_usser[0]='-';
  }
  if(dim.st_mode & S_IWUSR)
  {
    acces_usser[1]='w';
  }
   else{
    acces_usser[1]='-';
  }
  if(dim.st_mode & S_IXUSR)
  {
    acces_usser[2]='E';
  }
   else{
    acces_usser[2]='-';
  }
  char access_grup[3]="";
  if(dim.st_mode & S_IRGRP)
  {
    access_grup[0]='R';
  }
  else{
    access_grup[0]='-';
  }
  if(dim.st_mode & S_IWGRP )
  {
    access_grup[1]='W';
  }
  else{
    access_grup[1]='-';
  }
  if(dim.st_mode & S_IXGRP)
  {
    access_grup[2]='E';
  }
  else{
    access_grup[2]='-';
  }
    char access_altii[3]="";
     if(dim.st_mode & S_IROTH)
  {
    access_altii[0]='R';
  }
  else{
    access_altii[0]='-';
  }
  if(dim.st_mode & S_IWOTH  )
  {
    access_altii[1]='W';
  }
  else{
    access_altii[1]='-';
  }
  if(dim.st_mode & S_IROTH)
  {
    access_altii[2]='E';
  }
  else{
    access_altii[2]='-';
  }
    

  char buffer[2048];
  sprintf(buffer,"nume fisier:%s\n inaltime: %d\n, lungime : %d\n, dimensiune %d\n user id: %d\n numar legaturi: %d\n drepturi de acces useri:%s\n drepturi de acces de grup:%s\n drepturi de acces pt altii: %s\n",
	  argv[1],inaltime,latimea,dimensiune,user,nr_leg,acces_usser,access_grup,access_altii);
  // citire_director(argv[2]);
  write(f,buffer,strlen(buffer));
  
  if(close(fd)==-1)
    {
      perror("error close");
    exit(1);
    }
    close(f);*/
    citire_director(argv[1]);
    return 0;
}
