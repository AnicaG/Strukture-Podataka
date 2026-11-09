
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiranje strukture za jedan element stoga
typedef struct _Cvor {
    int broj;
    struct _Cvor* next;
} Cvor;

// Funkcija za dodavanje elementa na stog
Cvor* push(Cvor* top, int broj) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));

    // Provjera je li memorija uspjesno zauzeta
    if (novi == NULL) {
        printf("Greska kod alokacije memorije!\n");
        return top;
    }

    novi->broj = broj;
    novi->next = top; // Novi element povezuje se s dosadasnjim vrhom
    top = novi;       // Novi element postaje vrh stoga

    return top;
}

// Funkcija za skidanje elementa sa stoga
Cvor* pop(Cvor* top, int* broj) {
    Cvor* pomocni;

    // Provjera je li stog prazan
    if (top == NULL) {
        printf("Stog je prazan!\n");
        return NULL;
    }

    pomocni = top;          // Spremamo trenutni vrh
    *broj = top->broj;       // Uzimamo vrijednost vrha
    top = top->next;         // Pomicanje na sljedeci element

    free(pomocni);           // Oslobadanje memorije

    return top;
}

// Funkcija za izvodenje matematicke operacije
int racunaj(int a, int b, char operacija) {

    if (operacija == '+')
        return a + b;

    if (operacija == '-')
        return a - b;

    if (operacija == '*')
        return a * b;

    if (operacija == '/') {
        if (b == 0) {
            printf("Dijeljenje s nulom!\n");
            return 0;
        }

        return a / b;
    }

    return 0;
}

int main() {
    FILE* fp;
    Cvor* top = NULL; // Pokazivac na vrh stoga

    char izraz[100];
    char* token;

    int a, b, broj;
    int rezultat;

    // Otvaranje datoteke za citanje
    fp = fopen("izraz.txt", "r");

    // Provjera je li datoteka uspjesno otvorena
    if (fp == NULL) {
        printf("Greska kod otvaranja datoteke!\n");
        return -1;
    }

    // Citamo izraz iz datoteke
    if (fgets(izraz, sizeof(izraz), fp) == NULL) {
        printf("Datoteka je prazna!\n");
        fclose(fp);
        return -1;
    }

    // Zatvaranje datoteke
    fclose(fp);

    // Razdvajanje izraza na pojedine elemente
    token = strtok(izraz, " \n");

    // Obrada svakog elementa izraza
    while (token != NULL) {

        // Provjera je li element broj
        if (token[0] >= '0' && token[0] <= '9') {

            broj = atoi(token); // Pretvaranje teksta u broj

            // Stavljanje broja na stog
            top = push(top, broj);
        }

        // Ako element nije broj, onda je operator
        else {

            // Provjera ima li dovoljno elemenata na stogu
            if (top == NULL || top->next == NULL) {
                printf("Neispravan izraz!\n");

                // Oslobadanje preostalih elemenata
                while (top != NULL)
                    top = pop(top, &broj);

                return -1;
            }

            // Skidanje dva broja sa stoga
            top = pop(top, &b);
            top = pop(top, &a);

            // Izvodenje operacije
            rezultat = racunaj(a, b, token[0]);

            // Vracanje rezultata na stog
            top = push(top, rezultat);
        }

        // Prelazak na sljedeci element izraza
        token = strtok(NULL, " \n");
    }

    // Na kraju na stogu mora ostati samo jedan element
    if (top != NULL && top->next == NULL) {

        // Uzimanje konacnog rezultata
        top = pop(top, &rezultat);

        printf("Rezultat je: %d\n", rezultat);
    }
    else {
        printf("Neispravan izraz!\n");

        // Oslobadanje svih preostalih elemenata
        while (top != NULL)
            top = pop(top, &broj);
    }

    return 0;
}
