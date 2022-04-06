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

void cd (char *path)
{
    char* msg_eaccess="The search permission is denied for one of the components of path.\n";
    char* msg_namelong="The path is too long.\n";
    char* msg_notexist="The file does not exist.\n";

    if(chdir(path)!=0)
	{
        if(errno==EACCES)
            write(1,msg_eaccess,strlen(msg_eaccess));
        else if(errno==ENAMETOOLONG)
            write(1,msg_namelong,strlen(msg_namelong));
        else if(errno==ENOENT)
            write(1,msg_notexist,strlen(msg_notexist));
        else
            write(1,"ERROR !!!\n",10);

	}

}