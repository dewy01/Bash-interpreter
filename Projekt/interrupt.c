#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

void InterruptHandler (int signum) { 
    printf("You pressed ctrl+c\n");
    int fd = open("history.txt", O_CREAT | O_RDONLY, 0666);
    fprintf(stderr, "\n");
        for(int i = 0; i < 20; i++) {
        char* str = malloc(20 * sizeof(char));
        int length = 0;
            
        bool shouldBreak = false;
        char temp = '\n';
            
    	do {
    	    if(!read(fd, &temp, 1)) {
    	        shouldBreak = true;
    	        break;
    	    }
            str[length++] = temp;
        } while(temp != '\n');
        if(shouldBreak)
            break;
        fprintf(stderr, "%d. %s", i + 1, str);
    }
    
    fprintf(stderr, "Press ENTER to continue...");
    close(fd);
 }