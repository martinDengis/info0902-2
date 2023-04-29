/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>   // for sqrt function used in cmpPoint2d method

#include "BST2d.h"
#include "List.h"
#include "Point.h"

/* ========================================================================= *
 * STRUCTURES
 * ========================================================================= */
typedef struct BNode2d_t BNode2d;
struct BNode2d_t
{
    BNode2d *parent;
    BNode2d *left;
    BNode2d *right;   
    Point *key;
    void *value;
};

struct BST2d_t
{
    BNode2d *root;
    size_t size;
};

/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
/* Prototypes of static functions */
static BNode2d *bnNew2d(Point *point, void *value);

/* Prototypes */

/* ------------------------------------------------------------------------- *
 * Recursively frees all the nodes of the provided BST2d, along with their 
 * keys and values if the corresponding flag is set to true.
 * 
 * PARAMETERS
 * n            pointer to the root of the BST (or subtree) to free
 * freeKey      boolean flag indicating whether to free the keys or not
 * freeValue    boolean flag indicating whether to free the values or not
 * ------------------------------------------------------------------------- */

void bst2dFreeRec(BNode2d *n, bool freeKey, bool freeValue);

/* ------------------------------------------------------------------------- *
 * Recursively finds the set of positions (x,y) in the provided BST2d that 
 * are included in a ball of radius r and centered at the position q given 
 * as argument. Nodes that meet the criteria are added to a list.
 *
 * PARAMETERS
 * list         valid pointer to a list to add nodes that meet the criteria
 * depth	    depth of the current node in the subtree, initially 0
 * n            pointer to the current node being evaluated
 * q            pointer to the query point
 * r            The search radius
 * xBounds      pointer to a Point representing the x-coordinate bounds of q 
 *              (q->x - r ; q->x + r)
 * yBounds      pointer to a Point representing the y-coordinate bounds of q 
 *              (q->y - r ; q->y + r)
 * ------------------------------------------------------------------------- */

void bst2dBallSearchRec(List *list, int depth, BNode2d *n, Point *q, double r2, Point *xBounds, Point *yBounds);

/* ------------------------------------------------------------------------- *
 * Checks if a node is within a given radius r of a query point q.
 * If it is, its associated value is added to a list.
 * 
 * PARAMETERS
 * list     valid pointer to a list to add nodes that meet the criteria
 * n        pointer to the current node being evaluated
 * q        pointer to the query point
 * r        The search radius
 * ------------------------------------------------------------------------- */

void listAdmission (List *list, BNode2d *n, Point *q, double r2);

/* ------------------------------------------------------------------------- *
 * Compares the x coordinate of a given node's key with the lower and upper
 * bounds (x and y) of parameter xBounds to see if the x coordinate of the 
 * node belongs to the search range around the query point q.
 * Depending on the result of the comparison, the function recursively 
 * continues the exploration of subsequent branches in the tree.
 * 
 * < Exploration criteria on X >
 * If n->key->x <= xBounds->x : ignore left branch
 * If n->key->x >  xBounds->y : ignore right branch
 * Else : explore both branches
 * 
 * PARAMETERS
 * list         valid pointer to a list to add nodes that meet the criteria
 * n            pointer to the current node being evaluated
 * q            pointer to the query point
 * r            The search radius
 * depth	    depth of the current node in the subtree, called on even depths
 * xBounds      pointer to a Point representing the x-coordinate bounds of q 
 *              (q->x - r ; q->x + r)
 * yBounds      pointer to a Point representing the y-coordinate bounds of q 
 *              (q->y - r ; q->y + r) -> parameter never used in this function
 * ------------------------------------------------------------------------- */
void compareOnX(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds);

/* ------------------------------------------------------------------------- *
 * Compares the y coordinate of a given node's key with the lower and upper
 * bounds (x and y) of parameter yBounds to see if the y coordinate of the 
 * node belongs to the search range around the query point q.
 * Depending on the result of the comparison, the function recursively 
 * continues the exploration of subsequent branches in the tree.
 * 
 * < Exploration criteria on Y >
 * If n->key->y <= yBounds->x : ignore left branch
 * If n->key->y >  yBounds->y : ignore right branch
 * Else : explore both branches
 * 
 * PARAMETERS
 * list         valid pointer to a list to add nodes that meet the criteria
 * n            pointer to the current node being evaluated
 * q            pointer to the query point
 * r            The search radius
 * depth	    depth of the current node in the subtree, called on uneven depths
 * xBounds      pointer to a Point representing the x-coordinate bounds of q 
 *              (q->x - r ; q->x + r) -> parameter never used in this function
 * yBounds      pointer to a Point representing the y-coordinate bounds of q 
 *              (q->y - r ; q->y + r)
 * ------------------------------------------------------------------------- */

void compareOnY(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds);

/* ------------------------------------------------------------------------- *
 * Calculates the total depth of all nodes in the 2d binary search tree
 *
 * PARAMETERS
 * bst2d        2d binary search tree
 * n            pointer on a node (BNode type)
 * depth	    depth of the current node in the subtree, initially 0
 * totalDepth   pointer on the cumulated depth of nodes in the tree
 * nbNode 	    pointer on the number of nodes in the bst
 * ------------------------------------------------------------------------- */
void bst2dTotalNodeDepth(BST2d *bst2d, BNode2d *n, int depth, int *totalDepth, int *nbNode);

/* ------------------------------------------------------------------------- *
 * Compares two points based on their coordinates along a specified dimension.
 * 
 * PARAMETERS
 * p1       pointer to first Point to compare
 * p2       pointer to second Point to compare
 * depth	depth of the node to which p1 (or p2) is associated in the tree
 * 
 * RETURN
 * -1   If x (resp. y) coordinate of p1 < x (resp. y) coordinate of p2
 *  1	If x (resp. y) coordinate of p1 > x (resp. y) coordinate of p2 
 *  0 	If equal
 * ------------------------------------------------------------------------- */
int cmpPoint2d(Point *p1, Point *p2, int depth);

/* ------------------------------------------------------------------------- *
 * Calculates the Euclidean distance between two points in 2D space.
 *
 * PARAMETERS
 * p1       pointer to the first Point
 * p2       pointer to the second Point
 * 
 * RETURN
 * sqrt(dx * dx + dy * dy)  Euclidean distance between the two points as a double
 * ------------------------------------------------------------------------- */

double euclidianDistance(Point *p1, Point *p2);


/* ========================================================================= *
 * FUNCTIONS
 * ========================================================================= */
BNode2d *bnNew2d(Point *point, void *value)
{
    BNode2d *n = malloc(sizeof(BNode2d));
    if (n == NULL)
    {
        printf("bnNew: allocation error\n");
        return NULL;
    }
    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    n->key = point;
    n->value = value;
    return n;
}

BST2d *bst2dNew(){
    BST2d *bst2d = malloc(sizeof(BST2d));
    if (bst2d == NULL)
    {
        printf("bestNew: allocation error");
        return NULL;
    }
    bst2d->root = NULL;
    bst2d->size = 0;

    return bst2d;
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue)
{ 
    bst2dFreeRec(bst2d->root, freeKey, freeValue);
    free(bst2d);
}

void bst2dFreeRec(BNode2d *n, bool freeKey, bool freeValue) 
{
    if (n == NULL)
        return;
    
    bst2dFreeRec(n->left, freeKey, freeValue);
    bst2dFreeRec(n->right, freeKey, freeValue);
    if (freeKey)
        free(n->key);
    if (freeValue)
        free(n->value);
    free(n);
}

size_t bst2dSize(BST2d *bst2d)
{
    return bst2d->size;
}

bool bst2dInsert(BST2d *b2d, Point *key, void *value)
{
    // If tree is empty -> Assign root to newly created node
    if (b2d->root == NULL)
    {
        b2d->root = bnNew2d(key, value);
        if (b2d->root == NULL)
        {
            return false;
        }
        b2d->size++;
        return true;
    }

    BNode2d *prev = NULL;
    BNode2d *n = b2d->root;   // n is the pointer used to go through the tree (updated each time we pass to a new node)
    int depth = 0;
    
    while (n != NULL)
    {
        prev = n;   // temp var that will allow us to know where we stopped the search
        int cmp = cmpPoint2d(key, n->key, depth); // if < 0 : then go left, else : go right
        if (cmp < 0)
            n = n->left;    // Thus, at any point, n can take the NULL value that will exit the loop
        else if (cmp >= 0)
            n = n->right;

        depth++;
    }

    BNode2d *new = bnNew2d(key, value); // new node to insert
    if (new == NULL)
        return false;

    new->parent = prev;
    if (cmpPoint2d(key, prev->key, depth - 1) < 0)    // if < 0 : insert on the left, else : insert on the right
        prev->left = new;
    else
        prev->right = new;

    b2d->size++;
    return true;
}

void *bst2dSearch(BST2d *b2d, Point *q)
{
    BNode2d *n = b2d->root;
    int depth = 0;

    while (n != NULL)
    {
        if (ptCompare(n->key, q) == 0)  // If match between n->key and point, return the value associated to n
            return n->value;

        int cmp = cmpPoint2d(q, n->key, depth);   // If < 0 : go to left child, else : right child
        if (cmp < 0)
            n = n->left;
        else if (cmp >= 0)
            n = n->right;

        depth++; 
    }

    return NULL;
}

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    List *kValues =  listNew();
    if (kValues == NULL){
        return NULL;
    }

    /* ------------------------------------------------------------------------- *
    * xBounds      pointer to a Point representing the x-coordinate bounds of q 
    *              (q->x - r ; q->x + r)
    * yBounds      pointer to a Point representing the y-coordinate bounds of q 
    *              (q->y - r ; q->y + r)
    * ------------------------------------------------------------------------- */
    Point *xBounds = ptNew(ptGetx(q) - r, ptGetx(q) + r);
    Point *yBounds = ptNew(ptGety(q) - r, ptGety(q) + r);
    bst2dBallSearchRec(kValues, 0,  bst2d->root, q, r, xBounds, yBounds);

    return kValues;
}

void bst2dBallSearchRec(List *list, int depth, BNode2d *n, Point *q, double r, Point *xBounds, Point *yBounds){
    // Initial call on bst2dBallSearchRec(kValues, 0,  bst2d->root, q, r, xBounds, yBounds);
    if (n != NULL){
        listAdmission(list, n, q, r);
        if (depth % 2 == 0)
            compareOnX(list, n, q, r, depth, xBounds, yBounds);
        else
            compareOnY(list, n, q, r, depth, xBounds, yBounds);
    }
}

void listAdmission (List *list, BNode2d *n, Point *q, double r){
    if (euclidianDistance(n->key,q) <= r){
        bool success = listInsertLast(list, n->value);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
    }
}

void compareOnX(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds){
    // updating depth to depth + 1 and going right                 
    if (ptGetx(n->key) <= ptGetx(xBounds))
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);

    // updating depth to depth + 1 and going left                 
    else if (ptGetx(n->key) > ptGety(xBounds))
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);

    // updating depth to depth + 1 and continue exploration on both branches                 
    else {
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);                
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);
    }
}

void compareOnY(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds) {
    // updating depth to depth + 1 and going right
    if (ptGety(n->key) <= ptGetx(yBounds))
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);

    // updating depth to depth + 1 and going left
    else if (ptGety(n->key) > ptGety(yBounds))
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);

    // updating depth to depth + 1 and continue exploration on both branches                 
    else {
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);                
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);
    }
}

double bst2dAverageNodeDepth(BST2d *bst2d){
    int *totalDepth = malloc(sizeof(int));
    int *nbNode = malloc(sizeof(int));
    if (totalDepth == NULL || nbNode == NULL) {
        printf("totalDepth || nbNode : allocation error\n");
        return 0;
    }

    *totalDepth = 0, *nbNode = 0;
    bst2dTotalNodeDepth(bst2d, bst2d->root, 0, totalDepth, nbNode);

    // average node depth = division of total depth by nb of nodes
    double avgNodeDepth = (double) (*totalDepth) / (*nbNode);

    free(totalDepth);
    free(nbNode);

    return avgNodeDepth;
}

void bst2dTotalNodeDepth(BST2d *bst2d, BNode2d *n, int depth, int *totalDepth, int *nbNode){
    // Initial call on bst2dTotalNodeDepth(bst2d, bst2d->root, 0, totalDepth, nbNode);
    if (n != NULL){
        int i = depth + 1;
        bst2dTotalNodeDepth(bst2d, n->left, i, totalDepth, nbNode);
        *totalDepth += (i-1);   // (i-1) because we are adding depth of current node, not depth of nodes to call recursively
        *nbNode += 1;
        bst2dTotalNodeDepth(bst2d, n->right, i, totalDepth, nbNode);
    }
}

int cmpPoint2d(Point *p1, Point *p2, int depth) {
    if(depth % 2 == 0){
        if (ptGetx(p1) < ptGetx(p2))
            return -1;
        else if (ptGetx(p1) > ptGetx(p2))
            return +1;
        else 
            return 0;
    }
    else {
        if (ptGety(p1) < ptGety(p2))
            return -1;
        else if (ptGety(p1) > ptGety(p2))
            return +1;
        else
            return 0;
    }
}

double euclidianDistance(Point *p1, Point *p2) {
    double dx = ptGetx(p1) - ptGetx(p2);
    double dy = ptGety(p1) - ptGety(p2);
    return sqrt(dx * dx + dy * dy);
}
