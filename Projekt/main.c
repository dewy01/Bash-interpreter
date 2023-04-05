#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "input.c"
#include "interrupt.c"
#include "history.c"
#include "potok.c"


int main(int argc, char **argv)
{
    char* username = getenv("USER");
    char* location = getenv("PWD");
    //Get the name of the host processor (char *name, size_t namelen)
    char* hostname = malloc(32 * sizeof(char));
    gethostname(hostname, 32);
    int fd_history;
    char** hist = malloc(20 * sizeof(char*));
    int historyIndex = 0;
    history(hist,historyIndex,fd_history);

    int stdin_redirect = dup(STDIN_FILENO);
    int stdout_redirect = dup(STDOUT_FILENO);
    // w momencie 1 argumentu pobieramy enviromental variable
    // czyli usera i sciezke na ktorej sie znajduje

    while(true) {
        //Reprezentuje ID procesu jako int
        pid_t process;
        bool shouldWait = true, toFile = false;
        int* pipes;
        char* filename;
        signal(SIGINT, InterruptHandler);
        printf("%s@%s:%s$ ", username, hostname, location);
        char* input = Wczytaj_tekst(stdin);
        save_history(hist,historyIndex,fd_history,input);
        // Robienie potoku
        int programCount;
        // Dzieli wpisany input na pomniejsze intrukcje podzielone znakiem "|"
        char** program = Potok(input, "|", &programCount);

        if(programCount > 1) {
            pipes = malloc(2 * (programCount - 1) * sizeof(int));
            for(int i = 0; i < programCount - 1; i++){
                int result = pipe(pipes + (2 * i));
            }
        }

        for(int i = 0; i < programCount; i++) {
            int argsCount;
            char** args = Potok(program[i], " ", &argsCount);
            if((argsCount - 2) >= 0 && strcmp(args[argsCount - 2], ">>") == 0) {
                filename = args[argsCount - 1];
                toFile = true;
                args[argsCount - 2] = NULL;
                args[argsCount - 1] = NULL;
                argsCount -= 2;
            }
            else
                toFile = false;
            if(strcmp(args[argsCount - 1], "&") == 0) {
                shouldWait = false;
            
                args[argsCount - 1] = NULL;
                argsCount--;
            }
            else
                shouldWait = true;
            
            if((process = fork()) == 0) {
                if(programCount > 1) {
                    if(i == 0) {
                        dup2(pipes[1], STDOUT_FILENO);
                    }
                    else if (i != programCount - 1) {
                        dup2(pipes[(i - 1) * 2], STDIN_FILENO);
                        dup2(pipes[(i * 2) + 1], STDOUT_FILENO);
                    }
                    else {
                        dup2(pipes[(i - 1) * 2], STDIN_FILENO);
                    }
            
                    for(int j = 0; j < programCount - 1; j++) {
                        close(pipes[(j * 2)]);
                        close(pipes[(j * 2) + 1]);
                    }
                }
                if(toFile) {
                    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                    dup2(fd, STDOUT_FILENO);
                }
                
                if(execvp(args[0], args) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            }
            else if(process == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        
        }

        if(programCount > 1) {
            for(int i = 0; i < programCount - 1; i++) {
                close(pipes[(i * 2)]);
                close(pipes[(i * 2) + 1]);
            }
        }
                   
        if(shouldWait) {
            for(int i = 0; i < programCount; i++) { 
                wait(NULL); 
            }
        }
        else
            printf("pid = %d\n", process);
                   
        dup2(stdout_redirect, STDOUT_FILENO);

    }
}
