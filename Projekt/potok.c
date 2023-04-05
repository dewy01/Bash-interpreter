#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

char** Potok(char* str, char* korektor, int* argsCount) {
    int size = 2;
    *argsCount = 0;
    //strtok dzieli na wyrazy przez separatory okreslone zmienna "korektor"
    char** args = malloc(size * sizeof(char*));
    char* ptr = strtok(str, korektor);
    //po przekazaniu inputu znak "|" dzieli naszalinie na poszczególne czesci
    
    for(int i = 0; ptr != NULL; i++) {
        if((i + 1) >= size) {
            size *= 2;
            args = realloc(args, size * sizeof(char*));
        }
        
        args[i] = ptr;
        args[i + 1] = NULL;
        
        ptr = strtok(NULL, korektor);
        (*argsCount)++;
    }
    
    return args;
}