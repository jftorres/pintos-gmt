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
    

command_list *parse(char *input, int *in_file, int *out_file) {
    // flag for if we're reading within quotes
    bool quoted = 0;
    // flags for reading text
    bool reading = 0;
    bool reading_in_file = 0;
    bool reading_out_file = 0;
    
    // in and out file names
    char in_file_path[MAX_TOKEN_LEN + 1];
    char out_file_path[MAX_TOKEN_LEN + 1];
    // zeroing out the arrays makes terminating reading easier
    memset(in_file_path, 0, MAX_TOKEN_LEN + 1);
    memset(out_file_path, 0, MAX_TOKEN_LEN + 1);

    // set up the initial command and list
    command *cmd = malloc(sizeof(command));
    memset(args, 0, (MAX_NUM_ARGS) * (MAX_TOKEN_LEN + 1));
    cmd->in_desc = 0;
    cmd->out_desc = 1;
    command_list *list_ptr = malloc(sizeof(command_list));
    list_ptr->this = cmd;
    list_ptr->next = 0;
    command_list *head_ptr = list_ptr;
    
    int i = 0;
    // index for string currently being read
    int str_i = 0;
    for(i = 0; i < len; i++) {
        char curr = input[i];
        
        if(curr == 0) {
            // reached the end
            end_curr_token(cmd, &reading, &reading_in_file, &reading_out_file);
            // ensure the list is terminated
            cmd->next = 0;
        } else if(curr == ' ' && !quoted) {
            // end the current argument, and set the flag to 0
            end_curr_token(cmd, &reading, &reading_in_file, &reading_out_file);
        } else if(curr == '"') {
            if(quoted) {
                quoted = 0;
                end_curr_token(cmd, 
                    &reading, &reading_in_file, &reading_out_file);
            } else {
                quoted = 1;
                if(!reading_in_file && !reading_out_file) {
                    reading = 1;
                }
            }
        } else if(curr == '|' && !quoted) {
            // end processing for the current command
            end_curr_token(cmd, &reading, &reading_in_file, &reading_out_file);
            
            // set up the new command
            cmd = malloc(sizeof(command));
            
            list_ptr->next = malloc(sizeof(command_list));
            list_ptr = list_ptr->next;
            list_ptr->this = cmd;
            list_ptr->next = 0;
        } else if(curr == '<' && !quoted) {
            end_curr_token(cmd, &reading, &reading_in_file, &reading_out_file);
            reading_in_file = 1;
        } else if(curr == '>' && !quoted) {
            end_curr_token(cmd, &reading, &reading_in_file, &reading_out_file);
            reading_out_file = 1;
        } else {
            if(reading) {
                cmd->args[cmd->num_args][str_i] = curr;
            } else if(reading_in_file) {
                in_file_path[str_i] = curr;
            } else if(reading_out_file) {
                out_file_path[str_i] = curr;
            }
            
            str_i++;
        }
    }
    
    // create the file descriptors
    int in_file_desc = open(in_file_path, O_RDONLY);
    if(in_file_desc != -1) {
        *in_file = in_file_desc;
    }
    
    int out_file_desc = open(out_file_path, O_CREAT|O_WRONLY);
    if(out_file_desc != -1) {
        *out_file = out_file_desc;
    }
    
    return &command_list;
}
            


// null terminates the current token, counting it if it's an argument
// note that flags are input as pointers so they can be changed
void end_curr_token(command *cmd, bool *reading,
        bool *reading_in_file, bool *reading_out_file) {
    if(*reading) {
        cmd->num_args++;
        *reading = 0;
    } else if(*reading_in_file) {
        *reading_in_file = 0;
    } else if(*reading_out_file) {
        *reading_out_file = 0;
    }
}
    