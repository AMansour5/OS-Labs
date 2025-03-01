#include "utility.h"

int main() {
    char cmd[100];
    while (1) {
        printf("lab1> ");
        gets(cmd);
        if (strcmp(cmd, "exit") == 0) {
            break;
        }
        system(cmd);
    }
    return 0;
}