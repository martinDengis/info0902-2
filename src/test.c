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

int cmpPoint(void *p1, void *p2);

int cmpPoint(void *p1, void *p2) {
    Point *point1 = (Point *)p1;
    Point *point2 = (Point *)p2;

    if (point1->x < point2->x)
        return -1;
    else if (point1->x > point2->x)
        return +1;
    else if (point1->y < point2->y)
        return -1;
    else if (point1->y > point2->y)
        return +1;
    else
        return 0;
}

int main(){

    BST *bst = bstNew(cmpPoint);

    Point *key = ptNew(3,3), *key2 = ptNew(4,4), *key3 = ptNew(5,5), *key4 = ptNew(1,1), *key5 = ptNew(6,6), *key6 = ptNew(6,8), *key7 = ptNew(6,5);

    Point *kmin = ptNew(1,0), *kmax = ptNew(4,6);

    int coucou = 3, coucou2 = 4, coucou3 = 5, coucou4 = 1, coucou5 = 6, coucou6 = 8, coucou7 = 65;
    int *value = &coucou, *value2 = &coucou2, *value3 = &coucou3, *value4 = &coucou4, *value5 = &coucou5, *value6 = &coucou6, *value7 = &coucou7;

    bstInsert(bst, key, value);
    bstInsert(bst, key2, value2);
    bstInsert(bst, key3, value3);
    bstInsert(bst, key4, value4);
    bstInsert(bst, key5, value5);
    bstInsert(bst, key6, value6);
    bstInsert(bst, key7, value7);

    // (3,3)
    Point *p = (Point *)bst->root->key;
    int *v = (int *)bst->root->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p->x, p->y, *v);

    // (4,4)
    BNode *bn1 = bst->root->right;
    Point *p1 = (Point *)bn1->key;
    int *v1 = (int *)bn1->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p1->x, p1->y, *v1);

    // (5,5)
    BNode *bn2 = bn1->right;
    Point *p2 = (Point *)bn2->key;
    int *v2 = (int *)bn2->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p2->x, p2->y, *v2);

    // (1,1)
    BNode *bn3 = bst->root->left;   
    Point *p3 = (Point *)bn3->key;
    int *v3 = (int *)bn3->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p3->x, p3->y, *v3);

    // (6,6)
    BNode *bn4 = bn2->right;   
    Point *p4 = (Point *)bn4->key;
    int *v4 = (int *)bn4->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p4->x, p4->y, *v4);

    // (6,8)
    BNode *bn5 = bn4->right;   
    Point *p5 = (Point *)bn5->key;
    int *v5 = (int *)bn5->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p5->x, p5->y, *v5); 

    // (6,5)
    BNode *bn6 = bn4->left;   
    Point *p6 = (Point *)bn6->key;
    int *v6 = (int *)bn6->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p6->x, p6->y, *v6); 

    double avgNodeDepth = bstAverageNodeDepth(bst);
    printf("TEST : %f\n", avgNodeDepth);
    
    List *testList = listNew();
    if (testList == NULL){
        printf("ERROR");
        exit(0);
    }
    testList = bstRangeSearch(bst, kmin, kmax);
    for (LNode *lnode = testList->head; lnode != NULL; lnode = lnode->next){
        printf("%d\n", *(int*)lnode->value);
    }

    free(testList);

    bstFree(bst, false, false);
    
    return 0;
}