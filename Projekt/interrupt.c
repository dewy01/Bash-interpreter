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
    printf("\nYou pressed ctrl+c\n"); // Powiadamia o wciśnięci CTRL+C
    int fd = open("history.txt", O_CREAT | O_RDONLY, 0666); // otiera plik history.txt
    char* str;
    fprintf(stderr, "\n"); // przechodzi do nowej linii w konsoli
        for(int i = 0; i < 20; i++) {
            //inizjalizuje zmienna str z dynamicznie zalokowana pamiecia
            str = calloc(32 , sizeof(char));
            // długosc aktualnie czytanego napisu
            int length = 0;
            // czy koniec aktualnej linii
            bool endOfLine = false;
            //czytany znak
            char temp;
            if(!read(fd, &temp, 1)) 
    	        endOfLine = true;
    	    
            str[length++] = temp;
            // jezeli EOF zamiast nowej linii to koniec wyswietlania
    	while(temp != '\n'){
            // jezeli napotkal koniec linii to konczy przypisywanie
            // napisu do tablicy i wyswietla zapisana w pliku history komende
    	    if(!read(fd, &temp, 1)) {
    	        endOfLine = true;
    	        break;
    	    }
            str[length++] = temp;
        }
        if(endOfLine){
            free(str);
            break;
           
        }
        // wyswietlanie historii
        fprintf(stderr, "%d. %s", i + 1, str);
        if(str!=NULL)
            free(str);
    }
    // Oczekiwanie na interakcje uzytkownika
    fprintf(stderr, "Press ENTER to continue...");
    close(fd);
 }