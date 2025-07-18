#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>


// Function declarations
void shellLoop();
void setupEnvironment();
void onChildExit(int sig);
char *shellReadLine();
char **shellSplitLine(char *line);
int shellExecute(char **args);
int shellLaunch(char **args);
int shellCd(char **args);
int shellExport(char **args);
int shellEcho(char **args);
int shellExit(char **args);
int shellHelp(char **args);