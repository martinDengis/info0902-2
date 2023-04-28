/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

#include <stdlib.h>
#include <stdio.h>

/* ========================================================================= *
 * STRUCTURES
 * ========================================================================= */
struct PointDct_t
{
    BST *bst;
};

typedef struct BNode_t BNode;
struct BNode_t
{
    BNode *parent;
    BNode *left;
    BNode *right;   
    void *key;
    void *value;
};


/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
int cmpPoint(void *p1, void *p2);


/* ========================================================================= *
 * FONCTIONS
 * ========================================================================= */
PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = malloc(sizeof(PointDct));
    if (pd == NULL)
    {
        printf("pdctCreate: allocation error\n");
        return NULL;
    }
    BST *tree = bstNew(cmpPoint);
    for (LNode *p = lpoints->head, *v = lvalues->head; p != NULL; p = p->next, v = v->next) {
        bstInsert(tree, p->value, v->value);
    }

    pd->bst = tree;

    double nd = bstAverageNodeDepth(pd->bst);
    printf("\n     bstAverageNodeDepth : %f\n", nd);

    return pd;
}

void pdctFree(PointDct *pd) {
    free(pd);
}

size_t pdctSize(PointDct *pd) {
    return bstSize(pd->bst);
}

void *pdctExactSearch(PointDct *pd, Point *p) {
    return bstSearch(pd->bst, p);
}

List *pdctBallSearch(PointDct *pd, Point *p, double r) {
    Point *keymin = ptNew(ptGetx(p) - r, ptGety(p) - r);
    Point *keymax = ptNew(ptGetx(p) + r, ptGety(p) + r);
    double r2 = r*r;

    List *bstRSearchList = bstRangeSearch(pd->bst, keymin, keymax);
    List *pdctBallSearchList = listNew();

    bool success = false;
    for (LNode *ln = bstRSearchList->head; ln != NULL; ln = ln->next) {
        BNode *bn = (BNode *)ln->value;
        if (ptSqrDistance(bn->key, p) <= r2){
            success = listInsertLast(pdctBallSearchList, bn->value);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
        }
    }

    return pdctBallSearchList;
}
