/* Determinati daca exista sau nu drum direct intre doua restaurante dintr-o retea de tip graf */

#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    int data;
    struct Node *next;
} NODE;

// pentru simplitate, folosim int-uri pentru a numi restaurantele/locațiile
// ex: 1 - restaurantul 1 și tot așa

// Structura pentru reprezentarea grafului
typedef struct graf
{
    int n;
    int *vizitat;
    struct Node **lista_adiacenta;
} GRAPH;

// Structura pentru stivă
typedef struct stiva
{
    int top;
    int capacitate;
    int *vector;
} STACK;


NODE *creare_nod(int n)
{
    NODE *newnode = malloc(sizeof(NODE));
    newnode->data = n;
    newnode->next = NULL;
    return newnode;
}

// Funcție pentru adăugarea unei muchii între două noduri
void adaugare_muchie(GRAPH *graph, int src, int dest)
{
    // Adăugăm destinația în lista de adiacență a sursei
    NODE *newnode = creare_nod(dest);
    newnode->next = graph->lista_adiacenta[src];
    graph->lista_adiacenta[src] = newnode;

    // Adăugăm sursa în lista de adiacență a destinației
    newnode = creare_nod(src);
    newnode->next = graph->lista_adiacenta[dest];
    graph->lista_adiacenta[dest] = newnode;
}

// Funcție pentru crearea unui graf cu un număr specificat de noduri
GRAPH *creare_graf(int n)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    g->n = n;
    g->lista_adiacenta = malloc(n * sizeof(NODE *));
    g->vizitat = malloc(sizeof(int) * n);

    // Inițializăm listele de adiacență și vectorul de noduri vizitate
    for (int i = 0; i < n; i++)
    {
        g->lista_adiacenta[i] = NULL;
        g->vizitat[i] = 0;
    }
    return g;
}

// Funcție pentru crearea unei stive
STACK *creare_stiva(int cap)
{
    STACK *s = malloc(sizeof(STACK));
    s->vector = malloc(cap * sizeof(int));
    s->top = -1;
    s->capacitate = cap;
    return s;
}

// Funcție pentru a adăuga un element în stivă
void push(int elem, STACK *stack)
{
    if (stack->top + 1 >= stack->capacitate) {
        printf("Stiva este plină!\n");
        return;
    }
    stack->top = stack->top + 1;
    stack->vector[stack->top] = elem;
}

// Funcție pentru parcurgerea grafului
void DFS(GRAPH *graph, STACK *stack, int nr_restaurant)
{
    NODE *adj_list = graph->lista_adiacenta[nr_restaurant];
    NODE *aux = adj_list;
    graph->vizitat[nr_restaurant] = 1;
    push(nr_restaurant, stack);

    // Parcurgem toate nodurile adiacente
    while (aux != NULL)
    {
        int con_restaurant = aux->data;
        if (graph->vizitat[con_restaurant] == 0)
            DFS(graph, stack, con_restaurant);
        aux = aux->next;
    }
}

// Funcție pentru a adăuga toate muchiile în graf
void adaugare_muchii(GRAPH *g, int edg_nr, int nrv)
{
    int src, dest, i;
    printf("Adauga %d muchii (de la 0 la %d)\n", edg_nr, nrv-1);
    for (i = 0; i < edg_nr; i++)
    {
        scanf("%d%d", &src, &dest);
        adaugare_muchie(g, src, dest);
    }
}

// Funcție pentru resetarea vectorului de vizitare
void wipe(GRAPH *g, int nrv)
{
    for (int i = 0; i < nrv; i++)
    {
        g->vizitat[i] = 0;
    }
}

// Funcție pentru a verifica dacă există un drum între două noduri
int verificare_drum(GRAPH *g, int nrv, int src_restaurant, int dest_restaurant)
{
    STACK *s = creare_stiva(2 * nrv);
    int found = 0;

    DFS(g, s, src_restaurant);

    if (g->vizitat[dest_restaurant] == 1) {
        found = 1;
    }

    free(s->vector);
    free(s);
    return found;
}

int main()
{
    int nrv;
    int edg_nr;
    int restaurant_start;
    int restaurant_destinatie;
    int result;

    printf("Introduceti numarul de restaurante: ");
    scanf("%d", &nrv);

    printf("Introduceti numarul de drumuri: ");
    scanf("%d", &edg_nr);

    GRAPH *g = creare_graf(nrv);
    adaugare_muchii(g, edg_nr, nrv);

    printf("Introduceti restaurantul de start: ");
    scanf("%d", &restaurant_start);

    printf("Introduceti restaurantul destinatie: ");
    scanf("%d", &restaurant_destinatie);

    result = verificare_drum(g, nrv, restaurant_start, restaurant_destinatie);

    if (result)
        printf("Exista drum intre restaurantul %d si restaurantul %d\n", restaurant_start, restaurant_destinatie);
    else
        printf("Nu exista drum intre restaurantul %d si restaurantul %d\n", restaurant_start, restaurant_destinatie);
    
    return 0;
}
