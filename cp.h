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


void cp(char * file1,char * file2)
{
    //2files 
    //Queremos hacer copy from fp1 to fp2
    FILE *fp1, *fp2 = NULL;
    //r because we are just gonna read this file
    fp1=fopen(file1,"r");
    //w because we are going to write here what we copy from fp1
    fp2=fopen(file2,"w");

    if(fp1==NULL){
      printf("ERROR: some file does not exist");
      exit(1);
    }
    char c;
   //reades the next character (EOF if end of file or error)
    while((c=fgetc(fp1))!=EOF)
    {
      //write c in fp2
      fputc(c,fp2);
    }
    fclose(fp1);
    fclose(fp2);
  
}
  //Que hace: copy