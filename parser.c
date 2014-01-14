#define MAX_TOKEN_LEN 255
#define MAX_NUM_ARGS 16
#define MAX_NUM_COMMANDS 16

typedef struct command {
    // The arguments to the command.
    // As is the convention, args[0] is the command name.
    char args[MAX_NUM_ARGS][MAX_TOKEN_LEN + 1];
    // The actual number of arguments provided.
    unsigned num_args;
} command;

typedef struct command_list {
    command *this;
    command_list *next;
} command_list;
    

command_list parse(char *input) {
    int num_cmds = 0;
    // flag for if we're reading within quotes
    bool quoted = 0;
    // flag for reading text
    bool reading = 0;
    int fd[2];

    // set up the initial command and list
    command *cmd = malloc(sizeof(command));
    cmd->in_desc = 0;
    cmd->out_desc = 1;
    command_list *list_ptr = malloc(sizeof(command_list));
    list_ptr->this = cmd;
    list_ptr->next = 0;
    command_list *head_ptr = list_ptr;
    for(i = 0; i < len; i++) {
        char curr = input[i];
        
        if(curr == 0) {
            // reached the end

            end_curr_argument(cmd);
            // and ensure the list is terminated
            cmd->next = 0;
        } else if(curr == ' ' && !quoted) {
            // end the current argument, and set the flag to 0
            if(reading) {
                end_curr_argument(cmd)
                reading = 0;
            }
        } else if(curr == '"') {
            if(quoted) {
                quoted = 0;
                end_curr_argument(cmd);
            } else {
                quoted = 1;
                reading = 1;
            }
        } else if(curr == '|' && !quoted) {
            // end processing for the current command
            if(reading) {
                cmd->args[cmd->num_args][cmd->arg_lens[cmd->num_args]] = 0;
                cmd->num_args++;
                reading = 0;
            }
            
            // set up the new command
            cmd = malloc(sizeof(command));
            
            list_ptr->next = malloc(sizeof(command_list));
            list_ptr = list_ptr->next;
            list_ptr->this = cmd;
            list_ptr->next = 0;
        } else if(curr == '<' && !quoted) {
            if(reading) {
                cmd->args[cmd->num_args][cmd->arg_lens[cmd->num_args]] = 0;
                cmd->num_args++;
                reading = 0;
            }
            


// null terminages the argument and counts it
void end_curr_argument(command *cmd) {
    cmd->args[cmd->num_args][cmd->arg_lens[cmd->num_args]] = 0;
    cmd->num_args++;
}
    