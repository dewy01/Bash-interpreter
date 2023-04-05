#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>


void history(char** hist,int historyIndex,int fd_history){
        // Stworzenie pliku zapisuje jego historie
        fd_history = open("history.txt", O_CREAT | O_TRUNC | O_RDONLY, 0666);
        // Indeksuje pamiiec dla 20 linijek
        for(int i = 0; i < 20; i++) {
            char* str = malloc(20 * sizeof(char));
            int length = 0;
            
            bool shouldBreak = false;
            char temp = '\n';
            // Gdy koniec break inaczej idz dalej
    	    do {
    	        if(!read(fd_history, &temp, 1)) {
    	            shouldBreak = true;
    	    	    break;
    	        }
                str[length++] = temp;
            } while(temp != '\n');
            if(shouldBreak)
                break;
            hist[i] = str;
            historyIndex = (historyIndex + 1) % 20;
        }
    
        close(fd_history);
}

void save_history(char** hist,int historyIndex,int fd_history, char* input){
        fd_history = open("history.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
        
            //skopiowanie wartosci z inputu do pliku
            hist[historyIndex] = malloc(strlen(input) + 2);
            strcpy(hist[historyIndex], input);
            strcat(hist[historyIndex], "\n");
        
        // resetowanie indexu po 20 linijkach, rozpoczecie nadpisywania
            historyIndex = (historyIndex + 1) % 20;     
        //jezeli linia pelna to zapisuje dalej jezeli nie to do momentu konca
            if(!hist[historyIndex]) {
                for(int i = 0; i < historyIndex; i++) {
                    char* data = hist[i];
                    write(fd_history, data, strlen(data));
                }
            }
            else {
                for(int i = 0; i < 20; i++) {
                    char* data = hist[(historyIndex + i) % 20];
                    write(fd_history, data, strlen(data));
                }
            }
        
            close(fd_history);
}