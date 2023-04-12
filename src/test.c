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
    Point *key4 = ptNew(1,1);

    int coucou = 3;
    int *value = &coucou;
    int coucou2 = 4;
    int *value2 = &coucou2;
    int coucou3 = 5;
    int *value3 = &coucou3;
    int coucou4 = 1;
    int *value4 = &coucou4;

    bstInsert(bst, key, value);
    bstInsert(bst, key2, value2);
    bstInsert(bst, key3, value3);
    bstInsert(bst, key4, value4);

    Point *p = (Point *)bst->root->key;
    int *v = (int *)bst->root->value;
    printf("%f\n", p->x);
    printf("%f\n", p->y);
    printf("%d\n", *v);

    BNode *bn1 = bst->root->right;
    Point *p1 = (Point *)bn1->key;
    int *v1 = (int *)bn1->value;
    printf("%f\n", p1->x);
    printf("%f\n", p1->y);
    printf("%d\n", *v1);

    BNode *bn2 = bn1->right;
    Point *p2 = (Point *)bn2->key;
    int *v2 = (int *)bn2->value;
    printf("%f\n", p2->x);
    printf("%f\n", p2->y);
    printf("%d\n", *v2);

    BNode *bn3 = bst->root->left;
    if (!bn3){
        printf("coucou");
        exit(0);
    }
    
    Point *p3 = (Point *)bn3->key;
    int *v3 = (int *)bn3->value;
    printf("%f\n", p3->x);
    printf("%f\n", p3->y);
    printf("OK");
    printf("%d\n", *v3);
  

    bstFree(bst, true, true);
    
    return 0;
}