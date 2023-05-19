#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

char* Wczytaj_tekst(FILE* file){
    int size = 32; // rozmiar zaalokowanej pamieci na tekst
    int length = 0; // dlugosc wczytanego tekstu
    char* str = calloc(size , sizeof(char)); // alokacja pamieci na tekst 32 znaki
    char temp;
    if(file == NULL)
        return NULL;
    while((temp=fgetc(file))!=EOF) {
        // temp = fgetc(file); // Funkcja odczytuje jeden znak ze wskazanego strumienia
        // Do momentu napotkania końca STDIN lub nowej linii
        if(temp == '\n') {
            break;
        }
        // jezeli wczytany tekst jest za dlugi to zwieksza ilosc pamieci
        if(length >= size) {
            size *= 2;
            str = realloc(str, size * sizeof(char)); 
        }
        // przypisuje do kolejnej wartosci napisu odczytany znak
        str[length++] = temp;
    }
    // zwraca wczytany napis
    return str;
}