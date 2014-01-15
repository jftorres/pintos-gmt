#include <stdio.h>

#define BUFFERSIZE 1024

typedef struct command {
    // The command to be executed.
    char name[MAX_TOKEN_LEN + 1];
    // The arguments to the command.
    char args[MAX_NUM_ARGS][MAX_TOKEN_LEN + 1];
    // The actual number of arguments provided.
    unsigned num_args;
    // The actual lengths of the arguments.
    unsigned[MAX_NUM_ARGS] arg_lens;
}

command * parse(char *input, int fd_in, int fd_out);

int main(int argc, char const *argv[])
{
	// Buffer for user inputed commands
	char buffer[BUFFERSIZE];

	while(1)
	{
		print_prompt();

		// Get commands from raw input
		if (fgets(buffer,BUFFERSIZE,stdin) != NULL){

			int fd_in;
			int fd_out;
			int * num

			command *command_list = parse(buffer, fd_in ,fd_out);
			execute_full_command(command_list);
		}
		else{
			printf("Buffer overflow Error \n");
		}

	}
	return 0;
}

void print_prompt()
{
	// System calls to get username and path to current directory
	char * login = system("getlogin");
	char * path = system("getcwd");
	printf("%s:%s>", login,path);
}