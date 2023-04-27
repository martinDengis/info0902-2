/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

#include <stdlib.h>
#include <stdio.h>
// A compléter
/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
struct PointDct_t
{
    BST *bst;
};

typedef struct VNode_t 
{
    Point *point;
    void *data; // Data is the actual value associated to the Point
} VNode;

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

/* L'idée sera de faire un premier filtrage des clées à l'aide de la fonction bstRangeSearch et ensuite de ne garder dans la liste filtrée que les positions effectivement dans la boule définie en argument.*/
List *pdctBallSearch(PointDct *pd, Point *p, double r) {
    Point *keymin = ptNew(ptGetx(p) - r, ptGety(p) - r);
    Point *keymax = ptNew(ptGetx(p) + r, ptGety(p) + r);
    double r2 = r*r;

    // bstRangeSearch returns a list of LNode, each LNode has an attribute "value" -> This value will be of type VNode defined at the beginning of this file
    List *bstRSearchList = bstRangeSearch(pd->bst, keymin, keymax);
    printf("Done !");
    List *pdctBallSearchList = listNew();

    bool success = false;
    for (LNode *ln = bstRSearchList->head; ln != NULL; ln = ln->next) {
        VNode *vn = (VNode *)ln->value;
        if (ptSqrDistance(vn->point, p) <= r2){
            success = listInsertLast(pdctBallSearchList, vn->data);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
        }
    }

    return pdctBallSearchList;
}
