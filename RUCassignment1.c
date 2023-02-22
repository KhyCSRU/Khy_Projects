/*
        Assignment 1 Basic OS implementation
        Khylei Neal -- RUID: 203005542

        Functions:
                    help_cmd - This function prints a help menu

                    ls_cmd - This function uses the dirent.h library
                             header to list out the contents within a directory
                    cd_cmd - This function takes an arguement and will change your cwd

                    pwd_cmd - This function simply uses getcwd() to get and print the current directory path

                    rmdir_cmd - This function will remove a directory using the rmdir function

                    mkdir_cmd - this function will make a directory using the mkdir function




*/

#include <stdio.h>
#include <stdlib.h>   // used to execute subprocess and commands
#include <string.h>
#include <unistd.h>   // used for exit, getcwd,read, write, exec
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h> // for ls
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>  // used for open
void help_cmd(void);
void ls_cmd(void);
void cd_cmd(char **args);
void pwd_cmd(void);
void rmdir_cmd(char **args);
void mkdir_cmd(char **args);
int func_execute(char **args);


void help_cmd(void)
// Simply prints out a menu to show what the shell supports
{
    puts("List of Commands: ");
    printf("\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n", "cd directory - Change directory",
    "ls - lists information", "pwd - prints working directory", "mkdir directory - make directory", "rmdir directory - remove directory");
}

void ls_cmd(void)
// This function will print out all of the directories and files within the directory you are working on
{


    DIR *PNT_dir = opendir("."); // Open directory

    struct dirent *entry; // Point to entries
    if (entry == NULL) // Check if the directory was successfully opened
    {
        perror("opendir");
        return;
    }


    // Looping through all entries within the directory to display them
    while ((entry = readdir(PNT_dir)) != NULL)
        printf("%s\n", entry->d_name);

    closedir(PNT_dir); // Close directory
}

void cd_cmd(char **args)
// This function will change the directory you are working in
{
    if (args[1] == NULL)
        fprintf(stderr, "cd : error with syntax \n\n");
    else { // changes dir
        if (chdir(args[1]) != 0) // Check for error
            perror("cd");
    }
}

void pwd_cmd(void)
// This function will print the current working directory
{
    char curr_dir[1024];
    if (getcwd(curr_dir, sizeof(curr_dir)) != NULL) {
        printf("%s\n", curr_dir);
    } else // printing error if getcwd function is null
        perror("pwd");
}

void mkdir_cmd(char **args)
// This function will make a new directory as prompted
{
    if (args[1] == NULL) {
        fprintf(stderr, "mkdir : error with syntax \n");
    } else {
        if (mkdir(args[1], 0777) != 0) { // Checking error (mkdir function)
            perror("mkdir");
        }
    }
}

void rmdir_cmd(char **args)
// This function will remove the directory as prompted
{
    if (args[1] == NULL) {
        fprintf(stderr, "rmdir : error with syntax \n");
    } else {
        if (rmdir(args[1]) != 0) // Checking error
            perror("rmdir");

    }
}


int func_execute(char **args)
 // Taking parsed data and comparing the commands with strings to navigate to proper function
{
    if (args[0] == NULL)
    {
        return 1;
    } else if (strcmp(args[0], "help") == 0)
    {
        help_cmd();
    } else if (strcmp(args[0], "ls") == 0)
    {
        ls_cmd();
    } else if (strcmp(args[0], "cd") == 0)
    {
        cd_cmd(args);
    } else if (strcmp(args[0], "pwd") == 0)
    {
        pwd_cmd();
    } else if (strcmp(args[0], "mkdir") == 0)
    {
        mkdir_cmd(args);
    } else if (strcmp(args[0], "rmdir") == 0)
    {
        rmdir_cmd(args);
    } else if (strcmp(args[0], "exit") == 0)
    {
        printf("Exiting Now! Thank you!\n");
        return 0;
    } else
    {
        printf("%s: command not found\n", args[0]);
    }
    return 1;
}


char *read_line(void)
// read command line for the input line in shell
{
    char *line = NULL;
    size_t sz = 0;
    getline(&line, &sz, stdin); // reading commands to be parsed
    return line;
}

char **parse(char *line) {
    static char *toks[64]; // Storing all tokens in this array

    int track = 0;
    char *token = strtok(line, " \t\r\n");

    while (token != NULL && track < 63)
    { // Looping through all tokens in the array
        toks[track] = token;

        token = strtok(NULL, " \t\r\n");
        track++; // keeping track of index for tokens in array
    }

    toks[track] = NULL; // making last index NULL
    return toks;
}

int main(int argc, char** argv)
{
    printf("Welcome to Assignment 1 ! \n");
    puts("Type help for a list of commands that are supported\n");

    char *input, **args;
    int status;

    do {
    // Keeping the shell going until exit is entered
        printf("Shell>>> ");
        input = read_line();
        args = parse(input);

        status = func_execute(args);
    } while (status);

    return 0;
}
