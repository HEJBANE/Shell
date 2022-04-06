#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

void cat(char *file)
{
    //create filepointer
    FILE* fptr=NULL;
    fptr=fopen(file,"r");

    if(fptr==NULL)
    {
      printf("ERROR");
      exit(1);
    }
  char ch;
  
    while((ch=fgetc(fptr))!=EOF)
    {
      printf("%c",ch);
    }
    fclose(fptr);  
}