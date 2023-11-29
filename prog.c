#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<ctype.h>
#include <sys/wait.h>

char a[100]="";
void dimensiune(struct stat *st_file)
{
   sprintf(a,"dimensiune: %ld\n", st_file->st_size);
}
void utilizator(struct stat *st_file)
{
   sprintf(a,"utilizator %d\n", st_file->st_uid);
}
void modificare(struct stat *st_file)
{
   sprintf(a,"ultima modificare:%s\n", ctime(&st_file->st_mtime));
}
void drepturi_de_access(struct stat *dim)
{
  
   char acces_usser[4]="";
  if(dim->st_mode & S_IRWXU)
  {
    acces_usser[0]='R';
  }
  else{
    acces_usser[0]='-';
  }
  if(dim->st_mode & S_IWUSR)
  {
    acces_usser[1]='W';
  }
   else{
    acces_usser[1]='-';
  }
  if(dim->st_mode & S_IXUSR)
  {
    acces_usser[2]='X';
  }
   else{
    acces_usser[2]='-';
  }
  char access_grup[4]="";
  if(dim->st_mode & S_IRGRP)
  {
    access_grup[0]='R';
  }
  else{
    access_grup[0]='-';
  }
  if(dim->st_mode & S_IWGRP )
  {
    access_grup[1]='W';
  }
  else{
    access_grup[1]='-';
  }
  if(dim->st_mode & S_IXGRP)
  {
    access_grup[2]='X';
  }
  else{
    access_grup[2]='-';
  }
    char access_altii[4]="";
     if(dim->st_mode & S_IROTH)
  {
    access_altii[0]='R';
  }
  else{
    access_altii[0]='-';
  }
  if(dim->st_mode & S_IWOTH  )
  {
    access_altii[1]='W';
  }
  else{
    access_altii[1]='-';
  }
  if(dim->st_mode & S_IROTH)
  {
    access_altii[2]='X';
  }
  else{
    access_altii[2]='-';
  }
  strcpy(a,"drepturi de acces user: ");
  strcat(a,acces_usser);
  strcat(a,"\n");
  strcat(a,"drepturi de acces grup: ");
  strcat(a,access_grup);
  strcat(a,"\n");
  strcat(a,"drepturi de acces altii: ");
  strcat(a,access_altii);
  strcat(a,"\n");
}
void contor_leg(struct stat *st_file)
{
  sprintf(a, "%ld\n", st_file->st_nlink);
}

void executa(char *path,char *nume,struct stat *st_file,char *director2)
{

  int pid;
  int status;
  
 
    char buffer[500]="";
      DIR *director=opendir(director2);
  if(director == NULL)
    {
      perror("error open director");
      exit(1);
    }
   
  strcpy(buffer,"nume:");
  strcat(buffer,nume);
  strcat(buffer,"\n");
  //printf("in");
  //struct dirent *entry;
    char pathdir[200];
    sprintf(pathdir,"%s",director2);
    strcat(pathdir,"/");
    strcat(pathdir,nume);
    strcat(pathdir,"_statistica.txt");
    int f=creat(pathdir,O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
    if(f==-1)
      {
	perror("eroare creare fisier\n");
	exit(-1);
      }
  
  
  if(S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")==0)
  {
    pid=fork();
     if(pid<0)
    {
      perror("eroare pid\n");
      exit(-1);
    }
    if(pid==0)
    {

    
    //printf("in\n");
    char *file=path;
    int fd;
  fd=open(file, O_RDWR);
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
    int pixel=latimea*inaltime;
    char header[54];
    if((read(fd,header,sizeof(header)))!=sizeof(header))
    {
      perror("citire");
      exit(-1);
    }
    for(int i=0;i<pixel;i++)
    {
      unsigned char color[3];
      if((read(fd,color,sizeof(color)))!=sizeof(color))
      {
        perror("citire");
        exit(-1);

      }
      unsigned char gri=0.299*color[0]+0.587*color[1]+0.114*color[2];
      lseek(fd,-3,SEEK_CUR);
	    write(fd,&gri,sizeof(gri));
	    write(fd,&gri,sizeof(gri));
	    write(fd,&gri,sizeof(gri));
    }
    sprintf(a,"inaltime:%d \n latime:%d\n",inaltime,latimea);
    //strcat(buffer,inaltime);
    //strcat(buffer,"\n");
    //strcat(buffer,latimea);
    strcat(buffer,a);
    //struct stat st_file;
    if(close(fd)==-1)
	{
	  perror("error close");
	  exit(1);
	}
    dimensiune(st_file);
    strcat(buffer,a);
    utilizator(st_file);
    strcat(buffer,a);
    drepturi_de_access(st_file);
    strcat(buffer,a);
    contor_leg(st_file);
    strcat(buffer,a);
    
      exit(1);
    }
  
  }
  if(S_ISLNK(st_file->st_mode))
  {
    pid=fork();
    if(pid<0)
    {
      perror("eroare fork\n");
      exit(-1);
    }
    /*struct stat lst_file;
    if(lstat(path,&lst_file) == -1)
    {
      perror("stat error");
      exit(1);
    }
    */
    dimensiune(st_file);
    strcat(buffer,a);
    strcat(buffer,"\n");
    drepturi_de_access(st_file);
    strcat(buffer,a);
    exit(1);
    
  }
  if(S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp")!=0)
  {
    pid=fork();
    if(pid<0)
    {
      perror("eroare fork\n");
      exit(-1);
    }
    dimensiune(st_file);
    strcat(buffer,a);
    utilizator(st_file);
    strcat(buffer,a);
    drepturi_de_access(st_file);
    strcat(buffer,a);
    contor_leg(st_file);
    strcat(buffer,a);
    exit(2);
  }
  if(S_ISDIR(st_file->st_mode))
  {
    pid=fork();
    if(pid<0)
    {
      perror("eroare fork\n");
      exit(-1);
    }
    utilizator(st_file);
    strcat(buffer,a);
    drepturi_de_access(st_file);
    strcat(buffer,a);
    contor_leg(st_file);
    strcat(buffer,a);
    exit(3);
  }
   
  if(write(f,buffer,strlen(buffer))==-1)
    {
	    perror("eroare scriere fisier\n");
	    exit(-1);
    }
  if(close(f)==-1)
  {
    perror("eroare inchidere fisier\n");
    exit(-1);
  }
  
  
    while(pid=wait(&status)!=-1){
    if(WIFEXITED(status))
    {
      sprintf(a,"procesul cu pid:%d si codul:%d s-a incheiat\n",pid,WIFEXITED(status));
      /*DIR *director=opendir(director2);
  if(director == NULL)
    {
      perror("error open director");
      exit(1);
    }
    
      struct stat st_stat; 
      if(stat(director2,&st_stat) == -1)
	{
	  perror("out(statistici.txt) stat error");
	  exit(1);
	}*/
      if(write(f,a,strlen(a))==-1)
	{
	  perror("error scriere in fisier");
	  exit(-1);
	}
    }
      if(close(f)==-1)
	{
	  perror("error close "); 
	  exit(1);
	}
    }
  }
  

void citire_director(char *director1,char *director2)
{
  DIR *dir;
  
  dir=opendir(director1);
  //dir2=opendir(director2);
  if(dir==NULL)
    {
      perror("open director");
      exit(1);
    }
  
  
  struct dirent *entry;
  char path[300];
  while((entry=readdir(dir))!= NULL)
    {
      sprintf(path,"%s/%s",director1,entry->d_name);
      
      //printf("%s\n",entry->d_name);
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
          executa(path,entry->d_name,&st_file,director2);
          citire_director(path,director2);
        }
        else{
          //printf("else\n");
          executa(path,entry->d_name,&st_file,director2);
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
    citire_director(argv[1],argv[2]);
}