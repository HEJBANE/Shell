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


void pwd()
{
  char wd[1000];
  printf("Current working directory: %s \n", getcwd(wd,sizeof(wd)));
}

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


void touch(char * name)
{
  FILE* f;
  f= fopen(name, "w");
  fclose(f);
}

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

//We are going to write a function that will store our command (without the spaces) in a char ** Which will give:

static char	**read_args(char *raw_cmd, char *limit)
{
	char	*ptr = NULL;
	char	**cmd = NULL;
	size_t	idx = 0;

	ptr = strtok(raw_cmd, limit);

	while (ptr) {
		cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
		cmd[idx] = strdup(ptr);
		ptr = strtok(NULL, limit);
		++idx;
	}
	cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
	cmd[idx] = NULL;
	return (cmd);
}


//As we dynamically allocate our char **, we make a function (free_array) which will free our allocation.
static void	free_array(char **array)
{
	for (int i = 0; array[i]; i++) {
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

//To execute our command we will use the syscall execve.We must use the syscall fork to create a new process and launch our command in it.Which gives this:
static void	execute_cmd(char **cmd)
{
	pid_t	pid = 0;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0) {
		
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	} else {
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("shell");
	
	}
}

int	main(int argc, char ** argv)
{
	char	*buffer = NULL;
	size_t	buf_size = 2048;
  char ** cmd;
 // alloc buffer which will store the command entered by the user
	buffer = (char *)calloc(sizeof(char), buf_size);
	if (buffer == NULL) {
		perror("Malloc failure");
		return (EXIT_FAILURE);
	}

	// write a prompt
	write(1, "MyShell>> ", 10);

	// lecture de STDIN en boucle
	while (getline(&buffer, &buf_size, stdin) > 0) {
		cmd = read_args(buffer, " \n\t");
    
		if (cmd[0] == NULL)
			printf("Command not found\n");

    
		if(strcmp(cmd[0],"ls")==0){
      ls(".");
      }
    
     if(strcmp(cmd[0],"touch")==0){
        touch(cmd[1]);
       write(1, "MyShell/>>", 10);
       }

    if(strcmp(cmd[0],"cd")==0){
      cd(cmd[1]);
      }

    if(strcmp(cmd[0],"mv")==0){
    mv(cmd[1],cmd[2]);
      }
    
    if(strcmp(cmd[0],"cp")==0){
     cp(cmd[1],cmd[2]);
      }
  
    if(strcmp(cmd[0],"cat")==0){
    cat(cmd[1]);
      }
    
   if(strcmp(cmd[0],"pwd")==0){
    pwd();
      }
  

		write(1, "MyShell>> ", 10);
		free_array(cmd);

	}

	printf("Bye \n");
	free(buffer);
	}
