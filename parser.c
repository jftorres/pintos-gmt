#define MAX_TOKEN_LEN 255
#define MAX_NUM_ARGS 16
#define MAX_NUM_COMMANDS 16

typedef struct command {
    // The command to be executed.
    char name[MAX_TOKEN_LEN + 1];
    // The arguments to the command.
    char args[MAX_NUM_ARGS][MAX_TOKEN_LEN + 1];
    // The actual number of arguments provided.
    unsigned num_args;
    // The actual lengths of the arguments.
    unsigned[MAX_NUM_ARGS] arg_lens;
    // The file descriptors for input/output.
    int in_desc;
    int out_desc;
}
    

command * parse(char *input, int *num_commands) {
    commmand *cmds = malloc(MAX_NUM_COMMANDS * sizeof(command));
    char buf[MAX_NUM_ARGS][MAX_TOKEN_LEN+1];
    
    int num_args = 0;
    int num_cmds = 0;
    bool reading_args = 0;

    unsigned i;
    unsigned cmd_i = 0;
    for(i = 0; i < len; i++) {
        char curr = input[i];
        
        switch(curr) {
            // reached the end
            case 0:
                
            // end the current argument, and set the flag to 0
            case ' ':
                if(reading_args) {
                    num_args++;
                    token = 0;
                }
                break;
            case '|':
                
    