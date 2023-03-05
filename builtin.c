#include "builtin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAXSTR 4
#define MAXLEN 1024
char builtins[MAXSTR][MAXLEN];

//builtin functions
void bi_exit(char** args, int w_count){
    if(w_count-1 == 2){
        exit(atoi(args[1]));
    }else if(w_count-1 == 1){
        exit(0);
    }else{
        fprintf (stderr, "Too many args\n");
        return;
    }
}
void bi_envset(char** args, int w_count){
    if(w_count -1 < 3){
        //error
        fprintf (stderr, "Too few args\n");
    }else
    if(setenv(args[1],args[2], 1) <0){
        //error
        printf(stderr, "failed to set environment variable\n");
    }
}
void bi_envunset(char** args, int w_count){
    if(w_count -1 < 2){
        //error
        fprintf (stderr, "Too few args\n");
    }else
    if(unsetenv(args[1]) <0){
        //error
        printf(stderr, "failed to unset environment variable\n");
    }
}
void bi_cd(char** args, int w_count){
    if(w_count-1 == 2){
        if(chdir(args[1]) < 0)
            fprintf(stderr, "failed to change directories\n");
    }else if(w_count-1 == 1){
        chdir(getenv("HOME"));
    }else{
        //print error
        fprintf (stderr, "Too many args\n");
    }
}
void (*func_ptr[MAXSTR])(char** args, int w_count) = {bi_exit, bi_envset, bi_envunset, bi_cd};
//helper function
void buildlist(char** list){
    strncpy(builtins[0], "exit", strlen("exit"));
    strncpy(builtins[1], "envset", strlen("envset"));
    strncpy(builtins[2], "envunset", strlen("envunset"));
    strncpy(builtins[3], "cd", strlen("cd"));
}
//checks if first value of args is a built-in function and returns t/f
int check_builtin(char** args, int w_count){
    buildlist(builtins);

    int i =0;
    while(i < MAXSTR){
        if(strcmp(builtins[i++],args[0]) == 0)
            //return true
            return 1;
    } 
    //else return false
    return 0;
}
// executes one of the built in functions
void execute_builtin(char** args, int w_count){
    buildlist(builtins);
    int i = 0;
    while(i < MAXSTR){
        if(strcmp(builtins[i],args[0]) == 0){
            func_ptr[i](args, w_count);
        }
        i++;
    } 
}