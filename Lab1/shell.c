#include "shell.h"

int main() {
    shellLoop();
    return 0;
}

void shellLoop(){
    char *line;
    char **args;
    int status = 1;

    do {
        printf("> ");
        line = shellReadLine();
        args = shellSplitLine(line);
        status = shellExecute(args);
    } while (status);
}

char *shellReadLine(){
    char *line = malloc(100 * sizeof(char));
    scanf("%[^\n]", line);
    getchar();
    return line;
}

char **shellSplitLine(char *line){
    char **args = malloc(100 * sizeof(char *));
    char *token = strtok(line, " ");
    int i = 0;
    while (token != NULL)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
    return args;
}

int shellExecute(char **args){
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "cd") == 0) return shellCd(args);
    else if (strcmp(args[0], "exit") == 0) return shellExit(args);
    else if (strcmp(args[0], "help") == 0) return shellHelp(args);
    else if (strcmp(args[0], "export") == 0) return shellExport(args);
    else if (strcmp(args[0], "echo") == 0) return shellEcho(args);
    else return shellLaunch(args);
}

int shellLaunch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1) perror("Failed executing command");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) perror("Failed creating a child process");
    else
    {
        if (args[1] != NULL && strcmp(args[1], "&") == 0);
        else {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}

int shellCd(char **args){
    char* home = getenv("HOME");
    if (args[1] == NULL)
    {
        if (chdir(home) != 0) perror("Error changing directory");
        
    }
    else
    {
        if (args[1][0] == '$')
        {
            if (chdir(getenv(args[1] + 1)) != 0) perror("Error changing directory");   
        }
        else if (chdir(args[1]) != 0) perror("Erorr changing directory");
    }
    return 1;
}

int shellExit(char **args){
    return 0;
}

int shellHelp(char **args){
    printf("Shell implemented in C\n");
    printf("Commands:\n");
    printf("cd <directory> - change directory\n");
    printf("exit - exit the shell\n");
    printf("help - display help\n");
    return 1;
}

int shellExport(char **args){
    if (args[1] == NULL) fprintf(stderr, "shell: expected argument to \"export\"\n");
    else
    {
        if (putenv(args[1]) != 0) perror("Error exporting variable");
    }
    return 1;
}

int shellEcho(char **args){
    if (args[1] == NULL) fprintf(stderr, "shell: expected argument to \"echo\"\n");
    else
    {
        if (args[1][0] == '$') printf("%s\n", getenv(args[1] + 1));
        else printf("%s\n", args[1]);
    }
    return 1;
}

int shellNumBuiltins(){
    return 5;
}

