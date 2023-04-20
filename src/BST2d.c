/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST2d.h"
#include "List.h"
#include "Point.h"

// A compléter
struct Point_t
{
    double x;
    double y;
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

struct BST2d_t
{
    BNode *root;
    size_t size;

    // En créeant notre BST, on lui fournira la fonction de comparaison relative à ce BST qui permettra d'insérer les éléments
    int (*compfn)(void *, void *);
};

/* Prototypes of static functions */
static BNode *bnNew(Point *point, void *value);

// TODO : ajouter les prototypes manquants dans .h
void bst2dFreeRec(BNode *n, bool freeKey, bool freeValue);
void bst2dBallSearchRec(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds);
void listAdmission (List *list, BNode *n, Point *q, double r2);
void compareOnX(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds);
void compareOnY(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds);
void bst2dTotalNodeDepth(BST2d *bst2d, BNode *n, int depth, int *totalDepth, int *nbNode);
int cmpPoint(void *p1, void *p2);

// !!! Attention : pas le même premier argument que dans BST.c 
BNode *bnNew(Point *point, void *value)
{
    BNode *n = malloc(sizeof(BNode));
    if (n == NULL)
    {
        printf("bnNew: allocation error\n");
        return NULL;
    }
    n->left = NULL;
    n->right = NULL;
    n->key = point;
    n->value = value;
    return n;
}

/* On passe une fonction de comparaison en argument qui sera associée à un arbre bst2d qu'on créera dans le main. 
Cette fonction sera donc un attribut de l'arbre. 
Par après, quand on insérera des clefs dans l'arbre en question, notre fonction d'insertion prendra comme argument l'arbre bst2d (donc aussi la fonction de comparaison associée) et grâce à la fonction associée, insérera la clef au bon endroit */

BST2d *bst2dNew(int comparison_fn_t(void *, void *)){
    BST2d *bst2d = malloc(sizeof(BST2d));
    if (bst2d == NULL)
    {
        printf("bestNew: allocation error");
        return NULL;
    }
    bst2d->root = NULL;
    bst2d->size = 0;
    //On assigne la fonction de comparaison correspondant au type d'arbre avec lequel on travaille
    bst2d->compfn = comparison_fn_t;
    return bst2d;
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue)
{
    //Permet de libérer récursivement chaque noeud de l'arbre 
    bst2dFreeRec(bst2d->root, freeKey, freeValue);
    free(bst2d);
}

void bst2dFreeRec(BNode *n, bool freeKey, bool freeValue) 
{
    // Bool of freeKey & freeValue pcq on pourrait vouloir free que les values associées à nos clefs (pour changer le type de value par ex : de tripID à taxiID)
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
        b2d->root = bnNew(key, value);
        if (b2d->root == NULL)
        {
            return false;
        }
        b2d->size++;
        return true;
    }
    BNode *prev = NULL;
    BNode *n = b2d->root;   // n will be the pointer used to go through the tree (updated each time we pass to a new node)
    while (n != NULL)
    {
        prev = n;   // temp var that will allow us to know where we stopped the search
        int cmp = b2d->compfn(key, n->key);
        if (cmp <= 0)
        {
            n = n->left;    // Thus, at any point, n can take the NULL value that will exit the loop
        }
        else if (cmp > 0)
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
    if (b2d->compfn(key, prev->key) <= 0)
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
    BNode *n = b2d->root;
    while (n != NULL)
    {
        int cmp = b2d->compfn(q, n->key);
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

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    List *kValues =  listNew();
    if (kValues == NULL){
        return NULL;
    }
    Point *xBounds = ptNew(q->x - r, q->x + r);
    Point *yBounds = ptNew(q->y - r, q->y + r);
    double r2 = r*r;
    bst2dBallSearchRec(bst2d, kValues, 0,  bst2d->root, q, r2, xBounds, yBounds);

    return kValues;
}

void bst2dBallSearchRec(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds){
    // Appel initial sur bst2dBallSearchRec(bst2d, kValues, 0,  bst2d->root, q, r2, xBounds, yBounds);
    if (n != NULL){
        listAdmission(list, n, q, r2);
        if (depth % 2 == 0){
            compareOnX(bst2d, list, depth, n, q, r2, xBounds, yBounds);
        } 
        else {
           compareOnY(bst2d, list, depth, n, q, r2, xBounds, yBounds);
        }
    }
}

void listAdmission (List *list, BNode *n, Point *q, double r2){
    if (ptSqrDistance(n->key, q) <= r2){
        bool success = listInsertLast(list, n->value);
        if(!success){
            printf("Error while inserting value in list");
            exit(EXIT_FAILURE);
        }
    }
}

/* ------------------------------------------------------------------------- *
 * < Comparaison sur les X (resp. Y) >
 *
 * Si n->key->x < xBounds->x (resp. n->key->y <= yBounds->x): ignorer branche de gauche
 * Si n->key->x > xBounds->y (resp. n->key->y >= yBounds->y): ignorer branche de droite
 * Else, n'ignorer aucune branche
 * ------------------------------------------------------------------------- */
void compareOnX(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds){
    if (ptGetx(n->key) < xBounds->x) {
        bst2dBallSearchRec(bst2d, list, depth++ , n->right, q, r2, xBounds, yBounds);
    } 
    else if (ptGetx(n->key) >= xBounds->y) {
        bst2dBallSearchRec(bst2d, list, depth++ , n->left, q, r2, xBounds, yBounds);
    } 
    else {
        bst2dBallSearchRec(bst2d, list, depth++ , n->left, q, r2, xBounds, yBounds);
        bst2dBallSearchRec(bst2d, list, depth++ , n->right, q, r2, xBounds, yBounds);
    }
}

void compareOnY(BST2d *bst2d, List *list, int depth, BNode *n, Point *q, double r2, Point *xBounds, Point *yBounds){
    if (ptGety(n->key) < yBounds->x){
        bst2dBallSearchRec(bst2d, list, depth++ , n->right, q, r2, xBounds, yBounds);
    } 
    else if (ptGety(n->key) >= yBounds->y){
        bst2dBallSearchRec(bst2d, list, depth++ , n->left, q, r2, xBounds, yBounds);
    } 
    else {
        bst2dBallSearchRec(bst2d, list, depth++ , n->left, q, r2, xBounds, yBounds);
        bst2dBallSearchRec(bst2d, list, depth++ , n->right, q, r2, xBounds, yBounds);
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

void bst2dTotalNodeDepth(BST2d *bst2d, BNode *n, int depth, int *totalDepth, int *nbNode){
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
