/* ========================================================================= *
 * BST definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "List.h"
#include "Point.h"

/* ========================================================================= *
 * STRUCTURES
 * ========================================================================= */
/* Opaque Structure */
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

    int (*compfn)(void *, void *);
};


/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
/* Prototypes of static functions */
static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);

/* Prototypes added functions */
double bstAverageNodeDepth(BST *bst);

/* ------------------------------------------------------------------------- *
 * Calculates the total depth of all nodes in the binary search tree
 *
 * PARAMETERS
 * bst          binary search tree
 * n            pointer on a node (BNode type)
 * depth	    The depth of the bst	
 * totalDepth   pointer on the cumulated depth of nodes of the bst
 * nbNode 	    pointer on the number of nodes in the bst
 * ------------------------------------------------------------------------- */

void bstTotalNodeDepth(BST *bst, BNode *n, int depth, int *totalDepth, int *nbNode);

/* ------------------------------------------------------------------------- *
 * Checks if a node is in the range and adds it to a list if so
 *
 * PARAMETERS
 * bst          A binary search tree
 * list	        The list to be filled	
 * n            A pointer on a node (BNode type)
 * keymin	    Pointer on the lower bound of the range
 * keymax  	    Pointer on the upper bound of the range
 *
 * ------------------------------------------------------------------------- */

void setListBst(BST *bst, List *list, BNode *n, void *keymin, void *keymax);

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
BNode *bnNew(void *key, void *value)
{
    BNode *n = malloc(sizeof(BNode));
    if (n == NULL)
    {
        printf("bnNew: allocation error\n");
        return NULL;
    }
    n->left = NULL;
    n->right = NULL;
    n->key = key;
    n->value = value;
    return n;
}

BST *bstNew(int comparison_fn_t(void *, void *))
{
    BST *bst = malloc(sizeof(BST));
    if (bst == NULL)
    {
        printf("bestNew: allocation error");
        return NULL;
    }
    bst->root = NULL;
    bst->size = 0;
    bst->compfn = comparison_fn_t;
    return bst;
}

void bstFree(BST *bst, bool freeKey, bool freeValue)
{ 
    bstFreeRec(bst->root, freeKey, freeValue);
    free(bst);
}

void bstFreeRec(BNode *n, bool freeKey, bool freeValue) 
{
    if (n == NULL)
        return;
    
    bstFreeRec(n->left, freeKey, freeValue);
    bstFreeRec(n->right, freeKey, freeValue);
    if (freeKey)
        free(n->key);
    if (freeValue)
        free(n->value);
    free(n);
}

size_t bstSize(BST *bst)
{
    return bst->size;
}

bool bstInsert(BST *bst, void *key, void *value)
{
    // If tree is empty -> Assign root to newly created node
    if (bst->root == NULL)
    {
        bst->root = bnNew(key, value);
        if (bst->root == NULL)
        {
            return false;
        }
        bst->size++;
        return true;
    }
    BNode *prev = NULL;
    BNode *n = bst->root;   // n will be the pointer used to go through the tree (updated each time we pass to a new node)
    while (n != NULL)
    {
        prev = n;   // temp var that will allow us to know where we stopped the search
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0)
        {
            n = n->left;    // Thus, at any point, n can take the NULL value that will exit the loop
        }
        else if (cmp >= 0)
        {
            n = n->right;
        }
    }
    BNode *new = bnNew(key, value); // new node to insert
    if (new == NULL)
    {
        return false;
    }
    new->parent = prev;
    if (bst->compfn(key, prev->key) < 0)
    {
        prev->left = new;
    }
    else
    {
        prev->right = new;
    }
    bst->size++;
    return true;
}

void *bstSearch(BST *bst, void *key)
{
    BNode *n = bst->root;
    while (n != NULL)
    {
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0)
        {
            n = n->left;
        }
        else if (cmp > 0)
        {
            n = n->right;
        }
        else
        {
            return n->value;
        }
    }
    return NULL;
}

// A compléter
double bstAverageNodeDepth(BST *bst){
    int *totalDepth = malloc(sizeof(int));
    int *nbNode = malloc(sizeof(int));
    if (totalDepth == NULL || nbNode == NULL) {
        printf("totalDepth || nbNode : allocation error\n");
        return 0;
    }

    *totalDepth = 0, *nbNode = 0;
    bstTotalNodeDepth(bst, bst->root, 0, totalDepth, nbNode);

    // average node depth = division of total depth by nb of nodes
    double avgNodeDepth = (double) (*totalDepth) / (*nbNode);   

    free(totalDepth);
    free(nbNode);

    return avgNodeDepth;
}

void bstTotalNodeDepth(BST *bst, BNode *n, int depth, int *totalDepth, int *nbNode){
    // Initial call on bstTotalNodeDepth(bst, bst->root, 0, totalDepth, nbNode);
    if (n != NULL){
        int i = depth + 1;
        bstTotalNodeDepth(bst, n->left, i, totalDepth, nbNode);
        *totalDepth += (i-1);   // (i-1) because we are adding depth of current node, not depth of nodes to call recursively
        *nbNode += 1;
        bstTotalNodeDepth(bst, n->right, i, totalDepth, nbNode);
    }
}

List *bstRangeSearch(BST *bst, void *keymin, void *keymax){
    List *kValues =  listNew();
    if (kValues == NULL){
        return NULL;
    }
    setListBst(bst, kValues, bst->root, keymin, keymax);    // function to populate the list

    return kValues;
}

void setListBst(BST *bst, List *list, BNode *n, void *keymin, void *keymax){
    if (n != NULL){
        setListBst(bst, list, n->left, keymin, keymax); 

        // if n->key is in range [keymin : keymax] : add node to list
        if ((bst->compfn(n->key, keymax) <= 0) && (bst->compfn(n->key, keymin) >= 0)){
            bool success = listInsertLast(list, n);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
        }

        setListBst(bst, list, n->right,keymin, keymax);
    }
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
