/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

// A compléter
/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
struct PointDct_t
{
    BST *bst;
};

typedef struct BNode_t BNode;
struct BNode_t{
    BNode *parent;
    BNode *left;
    BNode *right;   
    void *key;
    void *value;
};

// typedef struct lnode_t
// {
//     void *value;
//     struct lnode_t *next;
// } LNode;

// typedef struct VNode_t 
// {
//     Point *point;
//     void *data; // Data is the actual value associated to the Point
// } VNode;

int cmpPoint(void *p1, void *p2);


/* ========================================================================= *
 * FONCTIONS
 * ========================================================================= */
PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = malloc(sizeof(PointDct*));
    if (pd == NULL)
    {
        printf("pdctCreate: allocation error\n");
        return NULL;
    }
    BST *tree = bstNew(cmpPoint);
    for (LNode *p = lpoints->head, *v = lvalues->head; v!= NULL; p = p->next, v = v->next) {
        bstInsert(tree, p->value, v->value);
    }

    // Checker la struct PointDct_t : je ne sais pas dans quelle mesure c'est toujours un dictionnaire (?). L'arbre EST le dictionnaire en soit...
    pd->bst = tree;
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
    return bstSearch(pd->bst, p);
}

/* L'idée sera de faire un premier filtrage des clées à l'aide de la fonction bstRangeSearch et ensuite de ne garder dans la liste filtrée que les positions effectivement dans la boule définie en argument.*/
List *pdctBallSearch(PointDct *pd, Point *p, double r) {                                                  
    Point *keymin = ptNew(ptGetx(p) - r, ptGety(p) - r);
    Point *keymax = ptNew(ptGetx(p) + r, ptGety(p) + r);
    double r2 = r*r;

    // bstRangeSearch returns a list of LNode, each LNode has an attribute "value" -> This value will be of type VNode defined at the beginning of this file
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
