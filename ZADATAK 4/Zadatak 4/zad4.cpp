#include <stdio.h>
#include <stdlib.h>

typedef struct Poli* Position;

typedef struct Poli {
    int coef;
    int pow;
    Position Next;
} polinom;

// Umetanje sortiranog elementa
void InsertSorted(Position head, Position newPoli)
{
    Position q = head;

    while (q->Next != NULL && q->Next->pow > newPoli->pow)
        q = q->Next;

    if (q->Next != NULL && q->Next->pow == newPoli->pow)
    {
        q->Next->coef += newPoli->coef;
        free(newPoli);
    }
    else
    {
        newPoli->Next = q->Next;
        q->Next = newPoli;
    }
}

// Citanje iz datoteke
int ReadFile(Position head, char* filename)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Greska kod otvaranja datoteke!\n");
        return -1;
    }

    int coef, pow;

    while (fscanf(fp, "%d %d", &coef, &pow) == 2)
    {
        Position newPoli = malloc(sizeof(polinom));

        if (newPoli == NULL)
        {
            fclose(fp);
            return -1;
        }

        newPoli->coef = coef;
        newPoli->pow = pow;
        newPoli->Next = NULL;

        InsertSorted(head, newPoli);
    }

    fclose(fp);
    return 0;
}

// Ispis polinoma
void PrintPoly(Position head)
{
    Position q = head->Next;

    while (q != NULL)
    {
        printf("%dx^%d ", q->coef, q->pow);
        q = q->Next;
    }

    printf("\n");
}

// Zbrajanje polinoma
void AddPoly(Position p, Position q, Position result)
{
    Position newPoli;

    p = p->Next;
    q = q->Next;

    while (p != NULL && q != NULL)
    {
        newPoli = malloc(sizeof(polinom));

        if (p->pow == q->pow)
        {
            newPoli->coef = p->coef + q->coef;
            newPoli->pow = p->pow;

            p = p->Next;
            q = q->Next;
        }
        else if (p->pow > q->pow)
        {
            newPoli->coef = p->coef;
            newPoli->pow = p->pow;

            p = p->Next;
        }
        else
        {
            newPoli->coef = q->coef;
            newPoli->pow = q->pow;

            q = q->Next;
        }

        newPoli->Next = NULL;

        if (newPoli->coef != 0)
            InsertSorted(result, newPoli);
        else
            free(newPoli);
    }

    while (p != NULL)
    {
        newPoli = malloc(sizeof(polinom));
        newPoli->coef = p->coef;
        newPoli->pow = p->pow;
        newPoli->Next = NULL;

        InsertSorted(result, newPoli);
        p = p->Next;
    }

    while (q != NULL)
    {
        newPoli = malloc(sizeof(polinom));
        newPoli->coef = q->coef;
        newPoli->pow = q->pow;
        newPoli->Next = NULL;

        InsertSorted(result, newPoli);
        q = q->Next;
    }
}

// Mnozenje polinoma
void MultiplyPoly(Position p, Position q, Position result)
{
    Position i, j, newPoli;

    for (i = p->Next; i != NULL; i = i->Next)
    {
        for (j = q->Next; j != NULL; j = j->Next)
        {
            newPoli = malloc(sizeof(polinom));

            newPoli->coef = i->coef * j->coef;
            newPoli->pow = i->pow + j->pow;
            newPoli->Next = NULL;

            InsertSorted(result, newPoli);
        }
    }
}

int main()
{
    polinom headP = { 0, 0, NULL };
    polinom headQ = { 0, 0, NULL };
    polinom headSum = { 0, 0, NULL };
    polinom headMul = { 0, 0, NULL };

    Position temp;

    if (ReadFile(&headP, "p.txt") != 0)
        return -1;

    if (ReadFile(&headQ, "q.txt") != 0)
        return -1;

    printf("Prvi polinom: ");
    PrintPoly(&headP);

    printf("Drugi polinom: ");
    PrintPoly(&headQ);

    AddPoly(&headP, &headQ, &headSum);

    printf("Zbroj: ");
    PrintPoly(&headSum);

    MultiplyPoly(&headP, &headQ, &headMul);

    printf("Umnozak: ");
    PrintPoly(&headMul);

    // Oslobadjanje memorije

    while (headP.Next != NULL)
    {
        temp = headP.Next;
        headP.Next = temp->Next;
        free(temp);
    }

    while (headQ.Next != NULL)
    {
        temp = headQ.Next;
        headQ.Next = temp->Next;
        free(temp);
    }

    while (headSum.Next != NULL)
    {
        temp = headSum.Next;
        headSum.Next = temp->Next;
        free(temp);
    }

    while (headMul.Next != NULL)
    {
        temp = headMul.Next;
        headMul.Next = temp->Next;
        free(temp);
    }

    return 0;
}