#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILOZOFOW 5
#define LEWY (i+FILOZOFOW-1)%FILOZOFOW
#define PRAWY (i+1)%FILOZOFOW

enum Stan { MYSLI, GLODNY, JE, SYTY };

enum Stan stany[FILOZOFOW];
pid_t filozof_procesy[FILOZOFOW];

void podnies_widelec(int i)
{
    stany[i] = GLODNY;
}

void odloz_widelec(int i)
{
    stany[i] = SYTY;
}

void jedz(int i)
{
    stany[i] = JE;
}

void mysl(int i)
{
    stany[i] = MYSLI;
}

void filozof(int i)
{
    while (1) {
        mysl(i);
        sleep(1);
        podnies_widelec(i);
        printf("Filozof %d: Podnosi widelec %d i %d\n", i, LEWY, i);
        sleep(1);
        jedz(i);
        printf("Filozof %d: Je\n", i);
        sleep(1);
        odloz_widelec(i);
        printf("Filozof %d: Odkłada widelec %d i %d\n", i, LEWY, i);
        sleep(1);
    }
}

int main()
{
    int i;

    for (i = 0; i < FILOZOFOW; i++) {
        stany[i] = MYSLI;
    }

 //ZA POMOCA FORK TWORZONA GALEZ PROCESU DLA KAZDEGO FILOZOFA
    for (i = 0; i < FILOZOFOW; i++) {
        pid_t pid = fork();

//GALEZ POTOMNA 
        if (pid == 0) {
            filozof(i);
            exit(0);
        } 
	//GALEZ RODZICIELSKA
	else if (pid > 0) {
            filozof_procesy[i] = pid;
        } else {
            printf("Błąd w funkcji fork()\n");
            exit(1);
        }
    }

    for (i = 0; i < FILOZOFOW; i++) {
        waitpid(filozof_procesy[i], NULL, 0);
    }

    return 0;
}