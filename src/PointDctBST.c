/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */

#include <stdlib.h>
#include <stdio.h>

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

/* ========================================================================= *
 * STRUCTURES
 * ========================================================================= */
struct PointDct_t
{
    BST *bst;
};

typedef struct NodeValue_t NodeValue;
struct NodeValue_t
{   
    Point *key;
    void *value;
};


/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * Compares 2 points on x then y
 *
 * PARAMETERS
 * p1        A void pointer (here : on a first point)
 * p2        A void pointer (here : on a second point)
 * 
 * RETURN
 *
 * -1 	     If x coordinate of p1 < x coordinate of p2 or if y coordinate of p1 < y coordinate of p2
 *  1		 If x coordinate of p1 > x coordinate of p2 or if y coordinate of p1 > y coordinate of p2
 *  0 	     If equal
 *
 * ------------------------------------------------------------------------- */

int cmpPoint(void *p1, void *p2);


/* ========================================================================= *
 * FUNCTIONS
 * ========================================================================= */
PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = malloc(sizeof(PointDct));
    if (pd == NULL)
    {
        printf("pdctCreate: allocation error\n");
        return NULL;
    }
    
    BST *tree = bstNew(cmpPoint);

    // Iterate through both arg lists and populate the tree structure
    bool success = false;
    for (LNode *p = lpoints->head, *v = lvalues->head; p != NULL; p = p->next, v = v->next) {
        NodeValue *nodeValue = malloc(sizeof(NodeValue));
        nodeValue->key = p->value;
        nodeValue->value = v->value;

        success = bstInsert(tree, p->value, nodeValue);
        if(!success){
            printf("Error while inserting value in tree");
            exit(EXIT_FAILURE);
        }
    }

    // Assign tree to dictionary
    pd->bst = tree;

    // Printing avg node depth for question 4
    double nd = bstAverageNodeDepth(pd->bst);
    printf("\n     bstAverageNodeDepth : %f\n", nd);

    return pd;
}

void pdctFree(PointDct *pd) {
    bstFree(pd->bst, false, false);
    free(pd);
}

size_t pdctSize(PointDct *pd) {
    return bstSize(pd->bst);
}

void *pdctExactSearch(PointDct *pd, Point *p) {
    NodeValue *nodeValue = bstSearch(pd->bst, p);
    if (nodeValue == NULL)
        return NULL;

    return nodeValue->value;
}

List *pdctBallSearch(PointDct *pd, Point *p, double r) {
    Point *keymin = ptNew(ptGetx(p) - r, ptGety(p) - r);
    Point *keymax = ptNew(ptGetx(p) + r, ptGety(p) + r);
    double r2 = r*r;

    // bstRangeSearch returns list as : if n->key is in range [keymin : keymax] -> add node to list
    List *bstRSearchList = bstRangeSearch(pd->bst, keymin, keymax);
    List *pdctBallSearchList = listNew();

    bool success = false;
    for (LNode *ln = bstRSearchList->head; ln != NULL; ln = ln->next) {
        NodeValue *nv = ln->value;
        if (ptSqrDistance(nv->key, p) <= r2){
            success = listInsertLast(pdctBallSearchList, nv->value);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
        }
    }

    ptFree(keymin);
    ptFree(keymax);
    listFree(bstRSearchList, false);

    return pdctBallSearchList;
}

int cmpPoint(void *p1, void *p2) {  
    Point *point1 = (Point *)p1;
    Point *point2 = (Point *)p2;

    if (ptGetx(point1) < ptGetx(point2))
        return -1;
    else if (ptGetx(point1) > ptGetx(point2))
        return +1;
    else if (ptGety(point1) < ptGety(point2))
        return -1;
    else if (ptGety(point1) > ptGety(point2))
        return +1;
    else
        return 0;
}
