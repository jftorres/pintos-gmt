/* This program will take in a linked list of arguments, the first one being the
   name of the program, the rest being arguments for that program to execute, and will
   execute that program with the given arguments by forkig a child process, and waiting
   for it to execute, and then returning to the parent process */


#define BUFFERSIZE 1024

// this is the main program which performs the execution
void execute_external(command *input, const char *in_file, const char *out_file){
	// Flag for the first command
	bool is_first_command = 1;

	int in_fd,out_fd; //input and output file descriptors

    // define a variable the wait function can return to  
    int status;
    pid_t pid;
    pid_t child;

    // Check if we have only one command
	if (is_first_command && is_last_command(input)){
		// No pipes involved

		// fork off the child process
	    if (pid = fork() < 0)
	    {
		switch(errno)
		{
	        // ADD MEANIGFUL MESSAGES PLUS ALL ERRORS HEREFIX FIX FIX

		case EAGAIN:
		    perror("sufficient memory could not be allocated for child");
		    exit(EXIT_FAILURE);
		}
	    }

	    // otherwise just execute. 
	    else if(pid == 0) // child process
	    {

		if (in_file != NULL)
		{
		    in_fd = open(in_file, O_RDONLY);

		    // check for open error
		    if (in_fd == -1)
		    {
			perror( "Error opening file" );
			printf( "Error opening file: %s\n", strerror( errno ) );
			exit(EXIT_FAILURE);
		    }

		    dup2(in_fd, STDIN_FILENO); /* Replace standard input */
		    
		    // TODO EXAMINE ALL POSSIBLE ERRORS HERE
		    if (close(in_fd) == -1)
		    {
			
		    }
		}


		if (out_file != NULL)
		{
		    out_fd = open(out_file, O_CREAT | O_TRUNC | O_WRONLY, 0);
		    dup2(out_fd, STDOUT_FILENO);       /* replace standard output */
		    dup2(out_fd, STDERR_FILENO);       /* replace standard error */

		    // TODO: DEAL WITH ERRORS HERE
		    if (close(out_fd) == -1)
		    {
			
		    }
		    
		}

		call_execve(input);

	    }

	    else //parent process
	    {
		// wait for child process to terminate
		wait(child, &status, 0);
		return;

	    }
		

	}
	// We keep track of the current command in the linked list
    command * curr_cmd = input;

    //  File descriptors of current pipe
    int pipefds[2];

    while(1){

    	// Check if we are on the first command but it is not the last
    	if (is_first_command) {
    		//Output is a pipe but input is not
    		pipe(pipefds);

    		if (fork() == 0){ //Child por current command
    			
    			close(pipefds[0]); /* Done with the read end */

    			// Replace standard out and error with write end of pipe
    			dup2(pipefds[1],STDOUT_FILENO);
    			dup2(pipefds[1],STDERR_FILENO);

    			close(pipefds[1]);

    			call_execve(curr_cmd);  /* Done with the write end */

    		}
    		else{
    			close(pipefds[1]);  /* We now are done with the write end of the current pipe */
    			in_fd = pipefds[0];  /* We save the read end for the next commands input */

    			is_first_command = 0;  /* The first command is over */

    		}
    	}

    	// Check if we are on an intermediate command
    	else if (!is_last_command(curr_cmd)){
    		//Both input and output are pipes
    		pipe(pipefds);

    		if (fork() == 0){ //Child por current command
    			
    			close(pipefds[0]); /* Done with the read end */

    			// Replace standard in with previous pipes read end
    			// and standard out and error with write end of pipe
    			dup2(current_in_fd, STDIN_FILENO);
    			dup2(pipefds[1],STDOUT_FILENO);
    			dup2(pipefds[1],STDERR_FILENO);

    			close(pipefds[1]);  

    			call_execve(curr_cmd);  /* Done with the write end */

    		}
    		else{
    			close(current_in_fd);
    			close(pipefds[1]);  /* We now are done with the write end of the current pipe */

    			in_fd = pipefds[0];  /* We save the read end for the next commands input */
    		}
    	}


    	}
    	else{
    		// Input is a pipe but output is not
    		// fork off the child process
		    if (pid = fork() < 0)
		    {
			switch(errno)
			{
		        // ADD MEANIGFUL MESSAGES PLUS ALL ERRORS HEREFIX FIX FIX

			case EAGAIN:
			    perror("sufficient memory could not be allocated for child");
			    exit(EXIT_FAILURE);
			}
		    }

		    // otherwise just execute. 
		    else if(pid == 0) // child process
		    {
				dup2(in_fd,STDIN_FILENO);  // Change the last pipes output to the input
				close(in_fd);  

			if (out_file != NULL)
			{
			    out_fd = open(out_file, O_CREAT | O_TRUNC | O_WRONLY, 0);
			    dup2(out_fd, STDOUT_FILENO);       /* replace standard output */
			    dup2(out_fd, STDERR_FILENO);       /* replace standard error */

			    // TODO: DEAL WITH ERRORS HERE
			    if (close(out_fd) == -1)
			    {
				
			    }
			    
			}

			call_execve();

		    }

		    else //parent process
		    {
			// wait for child process to terminate
			wait(child, &status, 0);
			close(in_fd);  /* Close the last pipe */
			return; /* We are done executing */

		    }

		    /* Move onto the next command */
		    curr_cmd = curr_cmd->next;
    	}
   	}
}

bool is_last_command(command * cmd){
	if (cmd->next == NULL){
		return 1;
	}
	else{
		return 0;
	}
}

void call_execve(command * input){
	// String paths
	char * bin_path = "/bin/";
	char * curr_dir = system("getcwd");

	// Setting up program names and arguments for execution

	// We create the full path name to the file
	char bin_program_name[BUFFERSIZE];

	// Copy bin directory onto the program name
	strcpy(bin_program_name,bin_path);

	// Finish creating a complete path to the program in bin
	strcat(bin_program_name,input->args[0]);

	// We create the full path name to the file if it is in the 
	char curr_dir_program_name[BUFFERSIZE];

	// Copy current directory onto the program name
	strcpy(curr_dir_program_name,curr_dir);

	// Finish creating a complete path to the program in the current directory
	strcat(curr_dir_program_name,input->args[0]);

	char *envp[] = {NULL};

	// Add a null to the end of the argument list
	char ** arguments = malloc(sizeof(char *) * (input->num_args + 1));
	memcpy(arguments,args, sizeof(char *) * input->num_args);
	arguments[num_args] = NULL;

	// Execute the program, assuming it is in the bin
	if(execve(bin_program_name,arguments, envp) < 0)
	{
	    // TODO 
	    switch(errno)
	    {
	       
	    }
	}

	//If we get here then the first execve never terminated so the program wasn't in bin

	// Execute the program, assuming it is in the current directory
	if(execve(curr_dir_program_name,arguments, envp) < 0)
	{
	    // TODO 
	    switch(errno)
	    {
	       
	    }
	}
}




// test program
int main()
{
    // define a test command
    command *cmd = malloc(sizeof(command));
    cmd->num_args = 2;
    
    strncpy(cmd->args[0], "grep", 4);
    strncpy(cmd->args[1], "command", 7);

    char *file_input = "/home/michael/Documents/CS 124/pintos-gmt/external_process.c";

    execute_external(cmd, file_input, NULL);
}
