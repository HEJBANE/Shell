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



void mv(char *file, char *location)
{
	char newlocation[50];
	

		DIR *isD;
		isD = opendir(location);

		//CONTROL ERROR: if the directory doesn't exist, the file will be renamed with the word written in the third argument.
			//(the file will stay in the same directory)

		if(isD==NULL)
            {
		link(file,location);
		unlink(file);
		printf("File successfully renamed\n");
            }
	    else
            {
                char *ptrL;
                ptrL = getcwd(newlocation, 50); 
                strcat(newlocation,"/");				
                strcat(newlocation,location);
                strcat(newlocation,"/");
                strcat(newlocation,file);	
                if(rename(file,ptrL)!=-1)
                    printf("Successfully moved\n");
                else
		    //CONTROL ERROR: if there are the correct number of arguments, but the file we want to move form one dirctory to another,
		      // is not in the directory (does not exist or there are ortographical errors) we will print in to warn the user
		    printf("Error:File or Directory not found\n");
                closedir(isD);
            }
	

}