#include <stdio.h>

int somma(int a, int b) {
    return a + b;
}

int sottrazione(int a, int b) {
    return a - b;
}

int main() {
    int (*operazione)(int, int); // Puntatore a funzione

    operazione = somma;
    printf("Somma: %d\n", operazione(5, 3));

    operazione = sottrazione;
    printf("Sottrazione: %d\n", operazione(5, 3));

    return 0;
}