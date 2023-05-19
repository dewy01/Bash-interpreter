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
#include "clear.c"

extern bool clearing;


int main(int argc, char **argv)
{
    char* username = getenv("USER"); // nazwa uzytkownika
    char* location = getenv("PWD"); // sciezka biezacego katalogu
    char* hostname = calloc(32 , sizeof(char)); // nazwa hosta na ktorym dziala program
    char* input;
    char** arguments;
    char** program;
    gethostname(hostname, 32); // pobranie nazwy hosta
    int hist_fd; // deskryptor dla historii polecen
    char** hist = calloc(20 , sizeof(char*)); // tablica 2 wymiarowa z historia polecen
    int histCount = 0; // liczba polecen w historii
    int stdout_desc = dup(STDOUT_FILENO);
    // w momencie 1 argumentu pobieramy enviromental variable
    // czyli usera i sciezke na ktorej sie znajduje

    while(clearing == false) {
        //Reprezentuje ID procesu jako int
        pid_t process;
        // bg - czy odpalic w tle 
        // saveFile - czy do pliku przekierowanie
        bool background = true;
        bool saveFile = false;
        int* pipes; // tablica deksryptorow dla potokow
        char* filename; // nazwa pliku
        signal(SIGINT, InterruptHandler);
        signal(SIGTSTP, SigStopFree);
        // wyswietlenie sciezki do promptu
        printf("%s@%s:%s$ ", username, hostname, location);
        input = Wczytaj_tekst(stdin);
        histCount = save_history(hist,histCount,hist_fd,input);
        // zmienna przetrzymujaca ilosc roznych instrukcji
        // dzielonych znakiem potoku
        int commands;
        // Dzieli wpisany input na pomniejsze intrukcje podzielone znakiem "|"
        program = Potok(input, "|", &commands);
        //jezeli wiecej niz 1 instrukcja do wykonania
        if(commands > 1) {
            // alokowanie pamieci dla 2 roznych deskryptorow potokow
            pipes = malloc(2 * (commands - 1) * sizeof(int));
            for(int i = 0; i < commands - 1; i++){
                // tworzy dwa deskryptory plików dla każdego potoku: jeden do odczytu (deskryptor odczytu) i jeden do zapisu (deskryptor zapisu). 
                // Deskryptory te są zapisywane w odpowiednich komórkach tablicy pipes o indeksach (2 * i) i (2 * i) + 1 
                int result = pipe(pipes + (2 * i));
            }
        }

        for(int i = 0; i < commands; i++) {
            int args;
            //dzielenie kazdego argumentu na wyrazy po spacji
            // i przekierowanie ich do tablicy arguments
            arguments = Potok(program[i], " ", &args);
            // jezeli znak >> to wykonuje przekierowanie
            if((args - 2) >= 0 && strcmp(arguments[args - 2], ">>") == 0) {
                // odczytuje z argumentow nazwe pliku
                filename = arguments[args - 1];
                // flaga zapis do pliku na true
                saveFile = true;
                // znak >> i nazwa pliku zostala usunieta z tablicy
                arguments[args - 2] = NULL;
                arguments[args - 1] = NULL;
                args -= 2;
            }
            else
            // w innym wypadku nie zapisuj do pliku
                saveFile = false;
            // jezeli ostatni znak to & to wykonaj w tle
            if(strcmp(arguments[args - 1], "&")==0) {
                background = false;
                arguments[args - 1] = NULL;
                args--;
            }
            else
                background = true;
            // wywoluje fork ktory tworzy proces potomny
            if((process = fork()) == 0) {
                // jezeli wiecej niz 1 instrukcja to przekieruj
                // strumien do nastepnego programu przechowanego w pipes
                if(commands > 1) {
                    // jezeli jeden proces to przekierowujemy do pipes[1]
                    if(i == 0) {
                        dup2(pipes[1], STDOUT_FILENO);
                    }
                    // jesli to nie ostatni program to
                    else if (i != commands - 1) {
                        // strumien wejsciowy do nastepnego
                        dup2(pipes[(i - 1) * 2], STDIN_FILENO);
                        // struymien wyksciowy do nastepnego programu
                        dup2(pipes[(i * 2) + 1], STDOUT_FILENO);
                    }
                    else {
                        // w innym to juz koniec i strumien wejsciowy
                        // przkierowujemy do ostatniego programu
                        dup2(pipes[(i - 1) * 2], STDIN_FILENO);
                    }
                    // zamykanie nieuzywanych deskryptorow
                    // po zakonczeniu dzialania potokow
                    for(int j = 0; j < commands - 1; j++) {
                        close(pipes[(j * 2)]);
                        close(pipes[(j * 2) + 1]);
                    }
                }
                // jezeli saveFile true to przekierowanie wyjscia na plik
                if(saveFile) {
                    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                    dup2(fd, STDOUT_FILENO);
                }
                
                //uruchomienie polecenia w miejsce biezacego procesu
                // i wyswietlanie bledow
                if(execvp(arguments[0], arguments) == -1) {
                    perror("Błąd execvp");
                    exit(EXIT_FAILURE);
                }
            }
            // osbluga bledu
            else if(process == -1) {
                perror("Błąd fork()");
                exit(EXIT_FAILURE);
            }
            
        
        }

        if(commands > 1) {
            for(int i = 0; i < commands - 1; i++) {
                close(pipes[(i * 2)]);
                close(pipes[(i * 2) + 1]);
            }
        }
        // jesli background to true, proces czeka az 
        // poprzedni program zakonczy dzialanie          
        if(background) {
            for(int i = 0; i < commands; i++) { 
                wait(NULL); 
            }
        }
        //przywrocenie standardowego wyjscia
        // zakonczenie procesu wykonwyania programu
        dup2(stdout_desc, STDOUT_FILENO);

    }
    free(hostname);
    for(int i= 0; i<20;i++){
        if(hist[i]!=NULL)
            free(hist[i]);
    }
    if(hist!=NULL)
        free(hist);
    if(input!=NULL)
        free(input);
    for(int i= 0; i<(sizeof(arguments)/sizeof(arguments[0]));i++){
        if(arguments[i]!=NULL)
            free(arguments[i]);
    }
    if(arguments!=NULL)
        free(arguments);
    for(int i= 0; i<(sizeof(program)/sizeof(program[0]));i++){
        if(program[i]!=NULL)
            free(program[i]);
    }
    if(program!=NULL)
        free(program); 
}
