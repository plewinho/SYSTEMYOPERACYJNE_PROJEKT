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
    struct message msg, response;

    // Tworzenie klucza dla kolejki komunikatów
    key = ftok(".", 'A');

    // Tworzenie kolejki komunikatów
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Wczytywanie łańcucha tekstowego od użytkownika
    printf("Podaj łańcuch tekstowy: ");
    fgets(msg.mtext, sizeof(msg.mtext), stdin);
    msg.mtype = 1;

    // Wysyłanie wiadomości do serwera
    if (msgsnd(msgid, &msg, sizeof(struct message) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Wysłano wiadomość do serwera: %s\n", msg.mtext);

    // Odbieranie odpowiedzi od serwera
    if (msgrcv(msgid, &response, sizeof(struct message) - sizeof(long), 2, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Otrzymano odpowiedź od serwera: %s\n", response.mtext);

    return 0;
}