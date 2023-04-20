#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "BST2d.h"
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

struct BST2d_t
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

// int cmpBst2d(void *p1, void *p2, void *d);

// int cmpBst2d(void *p1, void *p2, void *d) {
//     Point *point1 = (Point *)p1;
//     Point *point2 = (Point *)p2;
//     int *depth = (int *)d;

//     if ((int) depth % 2 == 0){
//         if (point1->x < point2->x)
//             return -1;
//         else if (point1->x > point2->x)
//             return +1;
//         else if (point1->y < point2->y)
//             return -1;
//         else if (point1->y > point2->y)
//             return +1;
//         else
//             return 0;
//     } 
//     else {
//         if (point1->y < point2->y)
//             return -1;
//         else if (point1->y > point2->y)
//             return +1;
//         else if (point1->x < point2->x)
//             return -1;
//         else if (point1->x > point2->x)
//             return +1;
//         else
//             return 0;
//     }

// }

int main(){

    BST2d *b2d = bst2dNew(cmpPoint);

    Point *key = ptNew(3,3), *key2 = ptNew(4,4), *key3 = ptNew(5,5), *key4 = ptNew(1,1), *key5 = ptNew(6,6), *key6 = ptNew(6,8), *key7 = ptNew(6,5);

    int coucou = 3, coucou2 = 4, coucou3 = 5, coucou4 = 1, coucou5 = 6, coucou6 = 8, coucou7 = 65;
    int *value = &coucou, *value2 = &coucou2, *value3 = &coucou3, *value4 = &coucou4, *value5 = &coucou5, *value6 = &coucou6, *value7 = &coucou7;

    bst2dInsert(b2d, key, value);
    bst2dInsert(b2d, key2, value2);
    bst2dInsert(b2d, key3, value3);
    bst2dInsert(b2d, key4, value4);
    bst2dInsert(b2d, key5, value5);
    bst2dInsert(b2d, key6, value6);
    bst2dInsert(b2d, key7, value7);

    // (3,3)
    Point *p = (Point *)b2d->root->key;
    int *v = (int *)b2d->root->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p->x, p->y, *v);

    // (4,4)
    BNode *bn1 = b2d->root->right;
    Point *p1 = (Point *)bn1->key;
    int *v1 = (int *)bn1->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p1->x, p1->y, *v1);

    // (5,5)
    BNode *bn2 = bn1->right;
    Point *p2 = (Point *)bn2->key;
    int *v2 = (int *)bn2->value;
    printf("x : %f - y : %f - value : %d\n----------\n", p2->x, p2->y, *v2);

    // (1,1)
    BNode *bn3 = b2d->root->left;   
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

    List *testList = listNew();
    if (testList == NULL){
        printf("ERROR");
        exit(0);
    }
    Point *q = ptNew(1,0);
    testList = bst2dBallSearch(b2d, q, 2);
    int length = testList->size;
    printf("TO DEBUG");
    printf("%d", length);
    for (int i = 0; i < length; i++){
        printf("%d", i);
    }

    free(testList);

    bst2dFree(b2d, false, false);
    
    return 0;
}