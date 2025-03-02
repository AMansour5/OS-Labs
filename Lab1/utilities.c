#include "utilities.h"

char *removeQuotes(char *str){
    char *new_str = malloc(strlen(str) * sizeof(char));
    if (str[0] == '\"' || str[0] == '\''){
        int i = 1;
        while (str[i] != str[0]){
            new_str[i - 1] = str[i];
            i++;
        }
        new_str[i - 1] = '\0';
        return new_str;
    }
    else return str;
}