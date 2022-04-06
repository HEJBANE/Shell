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
#include "cat.h"
#include "cd.h"
#include "cp.h"
#include "ls.h"
#include "mv.h"
#include "pwd.h"
#include "touch.h"

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