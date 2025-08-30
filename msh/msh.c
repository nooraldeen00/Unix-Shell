#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#define WHITESPACE " \t\n"      // separate tokens in command line input
#define MAX_COMMAND_SIZE 100    // Maximum size of a command
#define MAX_NUM_ARGUMENTS 10    // Maximum number of arguments

int main(int arg_count, char **arg_values) 
{
    char error_message[30] = "An error has occurred\n"; 
    char *command_string = (char*) malloc(MAX_COMMAND_SIZE);  
    FILE* input_source = stdin;  
    int batch_mode = 0, commands_executed = 0;  

    // Handle batch mode (if there's a second argument, it's treated as the batch file)
    if (arg_count == 2) 
    {
        batch_mode = 1;  // Activate batch mode
        input_source = fopen(arg_values[1], "r");  // Open the batch file
        if (input_source == NULL) 
        {
            // Print error and exit if the file cannot be opened
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    } 
    else if (arg_count > 2) 
    {
        // Print error and exit if more than one argument is provided (incorrect usage)
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    // Infinite loop to keep processing commands until "exit" or EOF in batch mode
    while (1) 
    {
        if (!batch_mode) 
        { 
            // If in interactive mode, print the prompt
            printf("msh> ");
        }

        // Read the command input from either stdin or batch file
        if (!fgets(command_string, MAX_COMMAND_SIZE, input_source)) 
        {
            // If we reach the end of the batch file, handle EOF
            if (batch_mode && feof(input_source)) 
            { 
                // If no commands were executed, exit with an error
                if (commands_executed == 0) 
                {
                    exit(1); 
                } 
                else 
                {
                    // Otherwise, exit successfully
                    exit(0);
                }
            }
        }

        commands_executed++;  // Increment the command execution count

        char *token[MAX_NUM_ARGUMENTS];  // Array to hold individual command arguments
        int token_count = 0;

        char *argument_pointer;
        char *working_string = strdup(command_string);  // Create a working copy of the input command
        char *head_ptr = working_string;  // Keep the original pointer for later free()

        // Tokenize the input command string using defined whitespace characters
        while ((argument_pointer = strsep(&working_string, WHITESPACE)) != NULL && token_count < MAX_NUM_ARGUMENTS) 
        {
            // Duplicate each token (argument) into the token array
            token[token_count] = strndup(argument_pointer, MAX_COMMAND_SIZE);
            if (strlen(token[token_count]) == 0) 
            {
                token[token_count] = NULL;  // Skip empty tokens (such as extra spaces)
            } 
            else 
            {
                token_count++;  // Increment the token count
            }
        }

        // If no command is entered (empty input), skip further processing
        if (token[0] == NULL) 
        {
            free(head_ptr);
            continue;
        }

        // Handle the built-in commands
        if (strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0) 
        {
            // If exit/quit command has extra arguments, show an error
            if (token[1] != NULL) 
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            } 
            else 
            {
                exit(0);  // Exit the shell
            }
        } 
        else if (strcmp(token[0], "cd") == 0) 
        {
            // If cd command has wrong number of arguments, show an error
            if (token[1] == NULL || token[2] != NULL) 
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            } 
            else if (chdir(token[1]) < 0) 
            {
                // Attempt to change directory and show an error if it fails
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        } 
        else 
        {
            // non-built-in commands, fork a new process
            pid_t process_id = fork();
            if (process_id == -1) 
            {
                // Fork failed, show error and exit
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(EXIT_FAILURE);
            } 
            else if (process_id == 0) 
            {
                // Child process: handle output redirection
                for (int i = 1; i < token_count; i++) 
                {
                    if (token[i] == NULL) break;
                    if (strcmp(token[i], ">") == 0) 
                    {
                        // Handle errors for output redirection syntax
                        if (token[i + 1] == NULL || token[i + 2] != NULL) 
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(0);
                        }

                        // Open the output file for redirection
                        int output_file = open(token[i + 1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                        if (output_file < 0) 
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(0);
                        }

                        // Redirect stdout and stderr to the output file
                        dup2(output_file, STDOUT_FILENO);
                        dup2(output_file, STDERR_FILENO);
                        close(output_file);

                        // Nullify the redirection token, so it doesn't affect execution
                        token[i] = NULL;
                        break;
                    }
                }

                // Look for the executable in common paths
                const char *path_env[] = { "/bin/", "/usr/bin/", "/usr/local/bin/", "./" };
                int total_paths = sizeof(path_env) / sizeof(path_env[0]);

                for (int i = 0; i < total_paths; i++) 
                {
                    // Construct the executable path by prepending the directory
                    char executable_path[256];
                    snprintf(executable_path, sizeof(executable_path), "%s%s", path_env[i], token[0]);
                    
                    // If the executable is found and is executable, run it
                    if (access(executable_path, X_OK) == 0) 
                    {
                        execv(executable_path, token);
                        break;
                    }
                }

                // If execv fails, print an error and exit the child process
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(EXIT_SUCCESS);
            } 
            else 
            {
                // Parent process: wait for the child process to finish
                int child_status;
                waitpid(process_id, &child_status, 0);
            }
        }
        
        free(head_ptr);  // Free the working copy of the command string
    }

    free(command_string);  // Free the allocated memory for the command input
    return 0;
}
