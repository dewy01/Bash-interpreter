#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

bool clearing;


void SigStopFree (int signum) { 
    printf("\nYou pressed ctrl+z\n");
    clearing = true;
    fprintf(stderr, "Press ENTER to continue...");
 }