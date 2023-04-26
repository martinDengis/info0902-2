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

// A compléter
/* ========================================================================= *
 * PROTOTYPES
 * ========================================================================= */
/* Opaque Structure */
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

struct BST_t
{
    BNode *root;
    size_t size;

    // En créeant notre BST, on lui fournira la fonction de comparaison relative à ce BST qui permettra d'insérer les éléments
    int (*compfn)(void *, void *);
};

/* Prototypes of static functions */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);

double bstAverageNodeDepth(BST *bst);
void bstTotalNodeDepth(BST *bst, BNode *n, int depth, int *totalDepth, int *nbNode);
void setListBst(BST *bst, List *list, BNode *n, void *keymin, void *keymax);
int cmpPoint(void *p1, void *p2);

/* ========================================================================= *
 * FONCTIONS
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
    //On assigne la fonction de comparaison correspondant au type d'arbre avec lequel on travaille
    bst->compfn = comparison_fn_t;
    return bst;
}

void bstFree(BST *bst, bool freeKey, bool freeValue)
{
    //Permet de libérer récursivement chaque noeud de l'arbre 
    bstFreeRec(bst->root, freeKey, freeValue);
    free(bst);
}

void bstFreeRec(BNode *n, bool freeKey, bool freeValue) 
{
    // Bool of freeKey & freeValue pcq on pourrait vouloir free que les values associées à nos clefs (pour changer le type de value par ex : de tripID à taxiID)
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
    if (bst->compfn(key, prev->key) <= 0)
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

    double avgNodeDepth = (double) (*totalDepth) / (*nbNode);

    free(totalDepth);
    free(nbNode);

    return avgNodeDepth;
}

void bstTotalNodeDepth(BST *bst, BNode *n, int depth, int *totalDepth, int *nbNode){
    // Appel initial à faire sur bstTotalNodeDepth(*bst, bst->root, 0, 0, nbNode)
    if (n != NULL){
        int i = depth + 1;
        bstTotalNodeDepth(bst, n->left, i, totalDepth, nbNode);
        *totalDepth += (i-1);
        *nbNode += 1;
        bstTotalNodeDepth(bst, n->right, i, totalDepth, nbNode);
    }
}

// Cette fonction nous retourne enft toutes les valeurs dont les clés sont dans une certain tranche de x, avec un tri supplémentaire fait sur les y pour les x égaux. Donc, dans le pdctBallSearch, peut déjà récup toutes les paires de pts dont les x sont compris dans le rayon (et qq uns de ces points auront déjà été éliminés sur base de leur y)
List *bstRangeSearch(BST *bst, void *keymin, void *keymax){
    List *kValues =  listNew();
    if (kValues == NULL){
        return NULL;
    }
    setListBst(bst, kValues, bst->root, keymin, keymax);

    return kValues;
}

void setListBst(BST *bst, List *list, BNode *n, void *keymin, void *keymax){
    if (n != NULL){
        setListBst(bst, list, n->left, keymin, keymax); 
        
        if ((ptCompare(n->key, keymax) <= 0) && (ptCompare(n->key, keymin) >= 0)){
            bool success = listInsertLast(list, n->value);
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
