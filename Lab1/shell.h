#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// Function declarations
void shellLoop();
char *shellReadLine();
char **shellSplitLine(char *line);
int shellExecute(char **args);
int shellLaunch(char **args);
int shellCd(char **args);
int shellExit(char **args);
int shellHelp(char **args);
int shellNumBuiltins();