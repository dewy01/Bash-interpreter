#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

char** Potok(char* str, char* korektor, int* args) {
    // rozmiar 2 ponieważ jest to tablica wskaznikow do tablicy znakow
    // a kazdy wskaznik do lancucha tez zajmuje rozmiar
    int size = 2;
    *args = 0;
    //strtok dzieli na wyrazy przez separatory okreslone zmienna "korektor"
    char** arguments = calloc(size , sizeof(char*));
    char* ptr = strtok(str, korektor);
    //po przekazaniu inputu znak "|" dzieli naszalinie na poszczególne czesci
    
    // for iteruje przez podzielone wyrazy dopoki ptr czyli
    // zmienna przechowujaca napis nie bedzie NULL
    for(int i = 0; ptr != NULL; i++) {
        // jezeli liczba wyrazow wieksza to alokuje wiecej pamieci
        if((i + 1) >= size) {
            size *= 2;
            //realokacja pamieci do tablicy arguments na nowy rozmiar
            arguments = realloc(arguments, size * sizeof(char*));
        }
        // przypisanie do tablicy argumentow oddzielona czesc
        arguments[i] = ptr;
        // koniec wyrazu tablicy
        arguments[i + 1] = '\0';
        // kontynuacja podzialu napisu
        ptr = strtok('\0', korektor);
        // dodanie liczby argumentow aby potem poprawnie wywolac wykonanie programu
        (*args)++;
    }
    
    return arguments;
}