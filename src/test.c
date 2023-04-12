#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "List.h"
#include "Point.h"

typedef struct BNode_t BNode;

struct BNode_t
{
    BNode *parent;
    BNode *left;
    BNode *right;   
    void *key;
    void *value;
};

struct BST_t
{
    BNode *root;
    size_t size;

    // En créeant notre BST, on lui fournira la fonction de comparaison relative à ce BST qui permettra d'insérer les éléments
    int (*compfn)(void *, void *);
};

struct Point_t
{
    double x;
    double y;
};

int compare_doubles(void* a, void* b);
double bstAverageNodeDepth(BST *bst);
void bstTotalNodeDepth(BST *bst, BNode *n, int depth, int *totalDepth, int *nbNode);

int compare_doubles(void* a, void* b)
{
    double* a_ = (double*)a;
    double* b_ = (double*)b;
    return (*a_ > *b_) - (*a_ < *b_);
}

int main(){

    BST *bst = bstNew(compare_doubles);

    Point *key = ptNew(3,3);
    Point *key2 = ptNew(4,4);
    Point *key3 = ptNew(5,5);

    int coucou = 3;
    int *value = &coucou;
    int coucou2 = 4;
    int *value2 = &coucou2;
    int coucou3 = 5;
    int *value3 = &coucou3;

    bstInsert(bst, key, value);
    bstInsert(bst, key2, value2);
    bstInsert(bst, key3, value3);

    Point *p = (Point *)bst->root->key;
    printf("%f", p->x);
    printf("%f", p->y);

    int *v = (int *)bst->root->value;
    printf("%d", *v);

    bstFree(bst, true, true);
    
    return 0;
}