/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h> // For sqrt

#include "BST2d.h"
#include "List.h"
#include "Point.h"

// A compléter
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

/* Prototypes of static functions */
static BNode2d *bnNew2d(Point *point, void *value);

// TODO : ajouter les prototypes manquants dans .h
void bst2dFreeRec(BNode2d *n, bool freeKey, bool freeValue);
void bst2dBallSearchRec(List *list, int depth, BNode2d *n, Point *q, double r2, Point *xBounds, Point *yBounds);
void listAdmission (List *list, BNode2d *n, Point *q, double r2);
void compareOnX(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds);
void compareOnY(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds);
void bst2dTotalNodeDepth(BST2d *bst2d, BNode2d *n, int depth, int *totalDepth, int *nbNode);
int cmpPoint(Point *p1, Point *p2, int depth);
double euclidianDistance(Point *p1, Point *p2);

// !!! Attention : pas le même premier argument que dans BST.c 
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
    //Permet de libérer récursivement chaque noeud de l'arbre 
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
    BNode2d *n = b2d->root;   // n will be the pointer used to go through the tree (updated each time we pass to a new node)
    int depth = 0;
    while (n != NULL)
    {
        prev = n;   // temp var that will allow us to know where we stopped the search
        int cmp = cmpPoint(key, n->key, depth);
        if (cmp < 0)
        {
            n = n->left;    // Thus, at any point, n can take the NULL value that will exit the loop
            depth++;
        }
        else if (cmp >= 0)
        {
            n = n->right;
            depth++;
        }
    }
    BNode2d *new = bnNew2d(key, value); // new node to insert
    if (new == NULL)
        return false;
    new->parent = prev;
    if (cmpPoint(key, prev->key, depth - 1) < 0)
    {
        prev->left = new;
    }
    else
    {
        prev->right = new;
    }
    b2d->size++;
    return true;
}

void *bst2dSearch(BST2d *b2d, Point *q)
{
    BNode2d *n = b2d->root;
    int depth = 0;
    while (n != NULL)
    {
        if (ptCompare(n->key, q) == 0)
            return n->value;

        int cmp = cmpPoint(q, n->key, depth);
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
    Point *xBounds = ptNew(ptGetx(q) - r, ptGetx(q) + r);
    Point *yBounds = ptNew(ptGety(q) - r, ptGety(q) + r);
    bst2dBallSearchRec(kValues, 0,  bst2d->root, q, r, xBounds, yBounds);

    return kValues;
}

void bst2dBallSearchRec(List *list, int depth, BNode2d *n, Point *q, double r, Point *xBounds, Point *yBounds){
    // Appel initial sur bst2dBallSearchRec(kValues, 0,  bst2d->root, q, r, xBounds, yBounds);
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
    if (ptGetx(n->key) <= ptGetx(xBounds))
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);                
    else if (ptGetx(n->key) > ptGety(xBounds))
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);
    else {
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);                
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);
    }
}

void compareOnY(List *list, BNode2d *n, Point *q, double r, int depth, Point *xBounds, Point *yBounds) {
    if (ptGety(n->key) <= ptGetx(yBounds))
        bst2dBallSearchRec(list, depth + 1, n->right, q, r, xBounds, yBounds);
    else if (ptGety(n->key) > ptGety(yBounds))
        bst2dBallSearchRec(list, depth + 1, n->left, q, r, xBounds, yBounds);
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

    double avgNodeDepth = (double) (*totalDepth) / (*nbNode);

    free(totalDepth);
    free(nbNode);

    return avgNodeDepth;
}

void bst2dTotalNodeDepth(BST2d *bst2d, BNode2d *n, int depth, int *totalDepth, int *nbNode){
    // Appel initial à faire sur bst2dTotalNodeDepth(*bst2d, bst2d->root, 0, 0, nbNode)
    if (n != NULL){
        int i = depth + 1;
        bst2dTotalNodeDepth(bst2d, n->left, i, totalDepth, nbNode);
        *totalDepth += (i-1);
        *nbNode += 1;
        bst2dTotalNodeDepth(bst2d, n->right, i, totalDepth, nbNode);
    }
}

// fonction de comparaison
int cmpPoint(Point *p1, Point *p2, int depth) {
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