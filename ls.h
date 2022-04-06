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

void ls(const char *dir)
{
  struct dirent *d;
  DIR *mydir=opendir(dir); 
  struct stat mystat;

        if (!mydir) {
                printf("Usage: ls file location or new filename\n");
        } else {
             while((d = readdir(mydir)) != NULL)
              {
                if(d->d_name[0]=='.') continue;
                printf("%s ",d->d_name);
                printf(" ");
                }
          printf("\n");
         
           
        }
  closedir(mydir);
}