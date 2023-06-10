#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MAX_SIZE 100

struct message {
    long mtype;
    char mtext[MAX_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // Tworzenie klucza dla kolejki komunikatów
    key = ftok(".", 'A');
    
    // Tworzenie kolejki komunikatów
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Serwer uruchomiony. Oczekiwanie na wiadomość...\n");

    while (1) {
        // Odbieranie wiadomości od klienta
        if (msgrcv(msgid, &msg, sizeof(struct message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Otrzymano wiadomość od klienta: %s\n", msg.mtext);

        // Obliczanie długości łańcucha
        int length = strlen(msg.mtext);

        // Przygotowanie odpowiedzi do klienta
        struct message response;
        response.mtype = 2;
        snprintf(response.mtext, sizeof(response.mtext), "%d", length);

        // Wysyłanie odpowiedzi do klienta
        if (msgsnd(msgid, &response, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        printf("Wysłano odpowiedź do klienta: %s\n", response.mtext);
    }

    // Usuwanie kolejki komunikatów
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}