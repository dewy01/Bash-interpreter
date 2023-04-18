#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
char* Wczytaj_tekst(FILE* file){
    int size = 32;
    int length = 0;
    //alokacja pamięci na wczytany napis
    char* str = malloc(size * sizeof(char));
    char temp = '\n';
    //Dopoki nie ma konca linii to wczytuje po znaku do zalokowanej pamieci
    //Jezeli linia za dluga to realokuje pamiec na wieksza
    while((temp = fgetc(file)) != EOF && temp != '\n') {
        if(length >= size) {
            str = realloc(str, (size * 2) * sizeof(char));
            size *= 2;
        }
        str[length++] = temp;
    }
    // jezeli koniec pliku to przejdz do nowej linii i zakoncz
    if(temp == EOF) {
        printf("\n");
        exit(EXIT_SUCCESS);
    }
    // zwroc wczytany tekst
    return str;
    
}
*/
char* Wczytaj_tekst(FILE* file){
    int size = 32;
    int length = 0;
    char* str = malloc(size * sizeof(char));

    while(1) {
        char temp = fgetc(file);
        
        if(temp == EOF || temp == '\n') {
            if(temp == EOF) {
                printf("\n");
                exit(EXIT_SUCCESS);
            }
            break;
        }

        if(length >= size) {
            size *= 2;
            str = realloc(str, size * sizeof(char));
        }
        
        str[length++] = temp;
    }

    return str;
}