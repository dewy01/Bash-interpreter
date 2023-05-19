# InterpreterC
Interpreter bash w C

Aby włączyć program obsługujący interpreter poleceń należy uruchomić plik wykonywalny. 
Następnie jesteśmy przenoszeni do działania programu, który ma imitować rzeczywisty interpreter. W tym celu wyświetla nam użytkownika oraz jego ścieżkę. 
Program pozwala nam na używanie komend UNIX’owych za pomocą języka C, takich jak np. Ls: 
Do tego możemy utworzyć własne pliki lub przekierować do nich pewne komunikaty 
Program pozwala również na użycie potoku, oraz odpalenie instrukcji w tle za pomocą znaku “&” 
Oprócz tego program obsługuje SIGINT pokazując przy tym aktualną historię komend 
W celu zwolnienia pamięci przed opuszczeniem programu obsługujemy również SIGTSTP – czyli kombinację klawiszy ctrl+z 
Cała historia komend zapisywana jest na bieżąco również do pliku tekstowego o nazwie “history.txt” 

