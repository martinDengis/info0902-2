/* ========================================================================= *
 * PointDct definition (with BST2d)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST2d.h"

#include <stdio.h>
#include <stdlib.h>

// A compléter
struct PointDct_t{
    BST2d *bst2d;
};

int cmpPoint(void *p1, void *p2);

PointDct *pdctCreate(List *lpoints, List *lvalues){
    PointDct *pdctbst2d = malloc(sizeof(PointDct));
    if(!pdctbst2d){
        printf("pdctCreate: allocation error");
        return NULL;
    }

    BST2d *bst2dtoDct = bst2dNew();
    for(LNode *p = lpoints->head, *v = lvalues->head; p != NULL; p = p->next, v = v->next){
        bst2dInsert(bst2dtoDct, p->value, v->value);
    }


    pdctbst2d->bst2d = bst2dtoDct;

    // double nd = bst2dAverageNodeDepth(pdctbst2d->bst2d);
    // printf("\n-------\nbst2dAverageNodeDepth : %f\n-------\n", nd);
    return pdctbst2d;
}


void pdctFree(PointDct *pd){
    bst2dFree(pd->bst2d, false, false);
    free(pd);
}

size_t pdctSize(PointDct *pd){
    return bst2dSize(pd->bst2d);
}

void *pdctExactSearch(PointDct *pd, Point *p){
    return bst2dSearch(pd->bst2d, p);
}

List *pdctBallSearch(PointDct *pd, Point *p, double r){
    return bst2dBallSearch(pd->bst2d, p, r);
}