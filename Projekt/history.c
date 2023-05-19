#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

int save_history(char** hist,int histCount,int hist_fd, char* input){
        //otwarcie pliku
        hist_fd = open("history.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        char* data;
            //Rezerwacja pamieci dla nowego napisu w historii
            hist[histCount] = malloc(strlen(input) + 2);
            //skopiowanie wartosci z inputu do pliku i przejście do nowej linii
            strcpy(hist[histCount], input);
            strcat(hist[histCount], "\n");
        
        // resetowanie indexu po 20 linijkach, rozpoczecie nadpisywania
            histCount = (histCount + 1) % 20;     
        //jezeli linia jest pusta to nie nadpisujemy historii
            if(!hist[histCount]) {
                for(int i = 0; i < histCount; i++) {
                    data = hist[i];
                    write(hist_fd, data, strlen(data));    
                }
            }
            // w przeciwnym wypadku nadpisujemy komendy od poczatku pliku
            else {
                for(int i = 0; i < 20; i++) {
                    data = hist[(histCount + i) % 20];
                    write(hist_fd, data, strlen(data));
                }
            }
            
            close(hist_fd);
            return histCount;
}