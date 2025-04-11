#include <stdio.h>
#include <stdlib.h>

// Structura unui nod din lista de adiacenta
typedef struct Node {
    int data;
    struct Node *next;
} NODE;

// Structura grafului
typedef struct g {
    int v;
    int *vis;
    NODE **alst;
} GPH;

// Stiva pentru DFS
typedef struct s {
    int t;
    int scap;
    int *arr;
} STK;

NODE *create_node(int v) {
    NODE *nn = malloc(sizeof(NODE));
    nn->data = v;
    nn->next = NULL;
    return nn;
}

GPH *create_g(int v) {
    GPH *g = malloc(sizeof(GPH));
    g->v = v;
    g->alst = malloc(v * sizeof(NODE *));
    g->vis = calloc(v, sizeof(int));

    for (int i = 0; i < v; i++) {
        g->alst[i] = NULL;
    }

    return g;
}

void add_edge(GPH *g, int src, int dest) {
    NODE *nn = create_node(dest);
    nn->next = g->alst[src];
    g->alst[src] = nn;

    nn = create_node(src);
    nn->next = g->alst[dest];
    g->alst[dest] = nn;
}

STK *create_s(int scap) {
    STK *s = malloc(sizeof(STK));
    s->arr = malloc(scap * sizeof(int));
    s->t = -1;
    s->scap = scap;

    return s;
}

void push(int pshd, STK *s) {
    if (s->t < s->scap - 1) {
        s->arr[++s->t] = pshd;
    }
}

void wipe(GPH *g, int nrv) {
    for (int i = 0; i < nrv; i++) {
        g->vis[i] = 0;
    }
}

void DFS(GPH *g, STK *s, int v_nr) {
    NODE *aux = g->alst[v_nr];
    g->vis[v_nr] = 1;
    push(v_nr, s);

    printf("%d ", v_nr);

    while (aux != NULL) {
        int con_ver = aux->data;
        if (g->vis[con_ver] == 0) {
            DFS(g, s, con_ver);
        }
        aux = aux->next;
    }
}

void insert_edges(GPH *g, int edg_nr, int nrv) {
    int src, dest;
    printf("adauga %d munchii (de la 0 la %d)\n", edg_nr, nrv - 1);
    for (int i = 0; i < edg_nr; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(g, src, dest);
    }
}

void canbe(GPH *g, int nrv, STK *s1, STK *s2) {
    int found = 0;
    for (int i = 0; i < nrv && !found; i++) {
        wipe(g, nrv);
        s1->t = -1;
        DFS(g, s1, i);

        for (int j = 0; j < nrv && !found; j++) {
            wipe(g, nrv);
            s2->t = -1;
            DFS(g, s2, j);

            for (int x = 0; x <= s1->t && !found; x++) {
                for (int y = 0; y <= s2->t && !found; y++) {
                    if (s1->arr[x] == s2->arr[y]) {
                        found = 1;
                    }
                }
            }
        }
    }

    if (found) {
        printf("\nExista cel putin un nod accesibil din doua puncte.\n");
    } else {
        printf("\nNU exista noduri comune accesibile.\n");
    }
}

int main() {
    int nrv;
    int edg_nr;
    int vortex_1;
    int virtex_2;

    printf("cate noduri are graful?");
    scanf("%d", &nrv);

    printf("cate muchii are graful?");
    scanf("%d", &edg_nr);

    GPH *g = create_g(nrv);
    STK *s1 = create_s(2 * nrv);
    STK *s2 = create_s(2 * nrv);

    insert_edges(g, edg_nr, nrv);

    printf("introdu cele doua noduri pentru verificare:\n");
    scanf("%d%d", &vortex_1, &virtex_2);

    printf("DFS din nodul %d:\n", vortex_1);
    wipe(g, nrv);
    DFS(g, s1, vortex_1);

    printf("\nDFS din nodul %d:\n", virtex_2);
    wipe(g, nrv);
    DFS(g, s2, virtex_2);

    canbe(g, nrv, s1, s2);


    for (int i = 0; i < g->v; i++) {
        NODE *tmp = g->alst[i];
        while (tmp) {
            NODE *to_free = tmp;
            tmp = tmp->next;
            free(to_free);
        }
    }
    free(g->alst);
    free(g->vis);
    free(g);
    free(s1->arr);
    free(s1);
    free(s2->arr);
    free(s2);

    return 0;
}
