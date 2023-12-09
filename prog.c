#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>

char a[100] = "";
void dimensiune(struct stat *st_file)
{
  sprintf(a, "dimensiune: %ld\n", st_file->st_size);
}
void utilizator(struct stat *st_file)
{
  sprintf(a, "utilizator %d\n", st_file->st_uid);
}
void modificare(struct stat *st_file)
{
  sprintf(a, "ultima modificare:%s\n", ctime(&st_file->st_mtime));
}
void drepturi_de_access(struct stat *dim)
{

  char acces_usser[4] = "";
  if (dim->st_mode & S_IRWXU)
  {
    acces_usser[0] = 'R';
  }
  else
  {
    acces_usser[0] = '-';
  }
  if (dim->st_mode & S_IWUSR)
  {
    acces_usser[1] = 'W';
  }
  else
  {
    acces_usser[1] = '-';
  }
  if (dim->st_mode & S_IXUSR)
  {
    acces_usser[2] = 'X';
  }
  else
  {
    acces_usser[2] = '-';
  }
  char access_grup[4] = "";
  if (dim->st_mode & S_IRGRP)
  {
    access_grup[0] = 'R';
  }
  else
  {
    access_grup[0] = '-';
  }
  if (dim->st_mode & S_IWGRP)
  {
    access_grup[1] = 'W';
  }
  else
  {
    access_grup[1] = '-';
  }
  if (dim->st_mode & S_IXGRP)
  {
    access_grup[2] = 'X';
  }
  else
  {
    access_grup[2] = '-';
  }
  char access_altii[4] = "";
  if (dim->st_mode & S_IROTH)
  {
    access_altii[0] = 'R';
  }
  else
  {
    access_altii[0] = '-';
  }
  if (dim->st_mode & S_IWOTH)
  {
    access_altii[1] = 'W';
  }
  else
  {
    access_altii[1] = '-';
  }
  if (dim->st_mode & S_IROTH)
  {
    access_altii[2] = 'X';
  }
  else
  {
    access_altii[2] = '-';
  }
  strcpy(a, "drepturi de acces user: ");
  strcat(a, acces_usser);
  strcat(a, "\n");
  strcat(a, "drepturi de acces grup: ");
  strcat(a, access_grup);
  strcat(a, "\n");
  strcat(a, "drepturi de acces altii: ");
  strcat(a, access_altii);
  strcat(a, "\n");
}
void contor_leg(struct stat *st_file)
{
  sprintf(a, "%ld\n", st_file->st_nlink);
}
int suma = 0;
void executa(char *path, char *nume, struct stat *st_file, char *director2, char c)
{

  pid_t pid;
  int status;
  // printf("in\n");
  char buffer[500] = "";
  DIR *director = opendir(director2);
  if (director == NULL)
  {
    perror("error open director");
    exit(1);
  }

  // printf("in");
  // struct dirent *entry;

  if (S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp") == 0)
  {
    pid = fork();
    if (pid == -1)
    {
      perror("eroare proces\n");
      exit(-1);
    }
    if (pid == 0)
    {
      int count = 0;
      strcpy(buffer, "nume:");
      strcat(buffer, nume);
      strcat(buffer, "\n");
      count++;
      char pathdir[200];
      sprintf(pathdir, "%s", director2);
      strcat(pathdir, "/");
      strcat(pathdir, nume);
      strcat(pathdir, "_statistica.txt");
      int f = creat(pathdir, O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
      if (f == -1)
      {
        perror("eroare creare fisier\n");
        exit(-1);
      }
      char *file = path;
      int fd;
      fd = open(file, O_RDWR);
      if (fd == -1)
      {
        perror("error open");
        exit(1);
      }
      int latimea;
      int inaltime;
      lseek(fd, 18, SEEK_SET);

      if (read(fd, &latimea, sizeof(int)) != sizeof(int))
      {
        perror("error read");
        exit(1);
      }
      lseek(fd, 22, SEEK_SET);
      if (read(fd, &inaltime, sizeof(int)) != sizeof(int))
      {
        perror("error read");
        exit(1);
      }
      int pixel = latimea * inaltime;
      char header[54];
      if ((read(fd, header, sizeof(header))) != sizeof(header))
      {
        perror("citire");
        exit(-1);
      }
      pid_t pid0 = fork();
      if (pid0 == -1)
      {
        perror("eroare fork\n");
        exit(-1);
      }
      if (pid0 == 0)
      {
        for (int i = 0; i < pixel; i++)
        {
          unsigned char color[3];
          if ((read(fd, color, sizeof(color))) != sizeof(color))
          {
            perror("citire");
            exit(-1);
          }
          unsigned char gri = 0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2];
          lseek(fd, -3, SEEK_CUR);
          write(fd, &gri, sizeof(gri));
          write(fd, &gri, sizeof(gri));
          write(fd, &gri, sizeof(gri));
        }
        exit(1);
      }
      sprintf(a, "inaltime:%d \n latime:%d\n", inaltime, latimea);
      // strcat(buffer,inaltime);
      // strcat(buffer,"\n");
      // strcat(buffer,latimea);
      strcat(buffer, a);
      count = count + 2;
      // struct stat st_file;
      if (close(fd) == -1)
      {
        perror("error close");
        exit(1);
      }
      dimensiune(st_file);
      strcat(buffer, a);
      utilizator(st_file);
      strcat(buffer, a);
      drepturi_de_access(st_file);
      strcat(buffer, a);
      contor_leg(st_file);
      strcat(buffer, a);
      count = count + 6;

      if (write(f, buffer, strlen(buffer)) == -1)
      {
        perror("eroare scriere fisier\n");
        exit(-1);
      }
      if (close(f) == -1)
      {
        perror("eroare inchidere fisier\n");
        exit(-1);
      }
      exit(count);
    }
  }

  if (S_ISLNK(st_file->st_mode))
  {
    struct stat lst_file;
    if (lstat(path, &lst_file) == -1)
    {
      perror("stat error");
      exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
      perror("eroare proces\n");
      exit(-1);
    }
    if (pid == 0)
    {
      int count = 0;
      strcpy(buffer, "nume:");
      strcat(buffer, nume);
      strcat(buffer, "\n");
      count++;
      char pathdir[200];
      sprintf(pathdir, "%s", director2);
      strcat(pathdir, "/");
      strcat(pathdir, nume);
      strcat(pathdir, "_statistica.txt");
      int f = creat(pathdir, O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
      if (f == -1)
      {
        perror("eroare creare fisier\n");
        exit(-1);
      }
      dimensiune(st_file);
      strcat(buffer, a);
      dimensiune(&lst_file);
      strcat(buffer, a);
      strcat(buffer, "\n");
      drepturi_de_access(st_file);
      strcat(buffer, a);
      count = count + 5;

      if (write(f, buffer, strlen(buffer)) == -1)
      {
        perror("eroare scriere fisier\n");
        exit(-1);
      }
      if (close(f) == -1)
      {
        perror("eroare inchidere fisier\n");
        exit(-1);
      }
      exit(count);
    }
  }
  if (S_ISREG(st_file->st_mode) && strcmp(nume + strlen(nume) - 4, ".bmp") != 0)
  {
    // printf("regular in\n");
    int pfd[2];
    if (pipe(pfd) < 0)
    {
      perror("eroare deschidere pipe\n");
    }

    pid = fork();
    if (pid == -1)
    {
      perror("eroare proces\n");
      exit(-1);
    }
    if (pid == 0)
    {
      // printf("in first procces\n");
      int count = 0;
      strcpy(buffer, "nume:");
      strcat(buffer, nume);
      strcat(buffer, "\n");
      count++;
      char pathdir[200];
      sprintf(pathdir, "%s", director2);
      strcat(pathdir, "/");
      strcat(pathdir, nume);
      strcat(pathdir, "_statistica.txt");
      int f = creat(pathdir, O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
      if (f == -1)
      {
        perror("eroare creare fisier\n");
        exit(-1);
      }
      dimensiune(st_file);
      strcat(buffer, a);
      utilizator(st_file);
      strcat(buffer, a);
      drepturi_de_access(st_file);
      strcat(buffer, a);
      contor_leg(st_file);
      strcat(buffer, a);
      count = count + 6;

      if (write(f, buffer, strlen(buffer)) == -1)
      {
        perror("eroare scriere fisier\n");
        exit(-1);
      }
      if (close(f) == -1)
      {
        perror("eroare inchidere fisier\n");
        exit(-1);
      }
      close(pfd[0]);
      ssize_t x;
      char bufferpipe[1024];
      int fis;
      fis = open(path, O_RDONLY);
      if (fis == -1)
      {
        perror("deschidere fisier\n");
        exit(-1);
      }
      while ((x = read(fis, bufferpipe, sizeof(bufferpipe))) > 0)
      {
        // printf("citeste\n");
        write(pfd[1], bufferpipe, x);
      }
      close(pfd[1]);
      close(fis);
      exit(count);
    }
    int pfd2[2];
    if (pipe(pfd2) == -1)
    {
      perror("pipe\n");
      exit(-1);
    }
    pid_t pid2 = fork();
    if (pid2 == -1)
    {
      perror("eroare fork\n");
      exit(-1);
    }
    if (pid2 == 0)
    {
      // printf("in second pipe\n");
      close(pfd[1]);
      close(pfd2[0]);
      dup2(pfd[0], 0);
      close(pfd[0]);
      dup2(pfd2[1], 1);
      close(pfd2[1]);
      execlp("bash", "bash", "bashp.sh", c, NULL);
      perror("execlp\n");
      exit(-1);
    }
    close(pfd2[1]);
    close(pfd[1]);
    close(pfd[0]);
    FILE *st;
    int n;

    st = fdopen(pfd2[0], "r");
    while ((fscanf(st, "%d", &n)) != EOF)
    {
      suma = suma + n;
    }
    close(pfd2[0]);
  }
  if (S_ISDIR(st_file->st_mode))
  {
    pid = fork();
    if (pid == -1)
    {
      perror("eroare proces\n");
      exit(-1);
    }
    if (pid == 0)
    {
      int count = 0;
      strcpy(buffer, "nume:");
      strcat(buffer, nume);
      strcat(buffer, "\n");
      count++;
      char pathdir[200];
      sprintf(pathdir, "%s", director2);
      strcat(pathdir, "/");
      strcat(pathdir, nume);
      strcat(pathdir, "_statistica.txt");
      int f = creat(pathdir, O_WRONLY | O_CREAT | O_APPEND | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
      if (f == -1)
      {
        perror("eroare creare fisier\n");
        exit(-1);
      }
      utilizator(st_file);
      strcat(buffer, a);
      drepturi_de_access(st_file);
      strcat(buffer, a);
      contor_leg(st_file);
      strcat(buffer, a);
      count = count + 5;

      if (write(f, buffer, strlen(buffer)) == -1)
      {
        perror("eroare scriere fisier\n");
        exit(-1);
      }
      if (close(f) == -1)
      {
        perror("eroare inchidere fisier\n");
        exit(-1);
      }
      exit(count);
    }
  }
  while ((pid = wait(&status)) != -1)
  {
    if (WIFEXITED(status))
    {
      printf("Child with pid %d ended with status %d\n", pid, WEXITSTATUS(status));
    }
  }
}

void citire_director(char *director1, char *director2, char c)
{
  // printf("in\n");
  DIR *dir;

  dir = opendir(director1);
  // dir2=opendir(director2);
  if (dir == NULL)
  {
    perror("open director");
    exit(1);
  }

  struct dirent *entry;
  char path[300];
  while ((entry = readdir(dir)) != NULL)
  {
    sprintf(path, "%s/%s", director1, entry->d_name);

    // printf("%s\n",entry->d_name);
    // printf("while\n");

    if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
    {
      struct stat st_file;
      // printf("str\n");
      if (stat(path, &st_file) == -1)
      {
        perror("in stat error");
        exit(1);
      }
      if (S_ISDIR(st_file.st_mode))
      {
        executa(path, entry->d_name, &st_file, director2, c);
        citire_director(path, director2, c);
      }
      else
      {
        // printf("else\n");
        executa(path, entry->d_name, &st_file, director2, c);
      }
    }
  }

  if (closedir(dir) == -1)
  {
    perror("close dir");
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  // printf("main");
  if (argc != 4)
  {
    return 0;
  }
  char c = argv[3];
  citire_director(argv[1], argv[2], c);
  printf("au fost indentificatte in total %d propozitii corecte care contin litera %c\n", suma, c);
  return 0;
}