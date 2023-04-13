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
    if (bst2d->root == NULL)
    {
        bst2d->root = bnNew(key, value);
        if (bst2d->root == NULL)
        {
            return false;
        }
        bst2d->size++;
        return true;
    }
    BNode *prev = NULL;
    BNode *n = bst2d->root;   // n will be the pointer used to go through the tree (updated each time we pass to a new node)
    while (n != NULL)
    {
        prev = n;   // temp var that will allow us to know where we stopped the search
        int cmp = bst2d->compfn(key, n->key);
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
    if (bst2d->compfn(key, prev->key) <= 0)
    {
        prev->left = new;
    }
    else
    {
        prev->right = new;
    }
    bst2d->size++;
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
    double r2 = r*r;
    setListBst2d(bst2d, kValues, bst2d->root, q, r2);

    return kValues;
}

void setListBst2d(BST2d *bst2d, List *list, BNode *n, Point *q, double r2){
    if (n != NULL){
        if (ptSqrDistance(n->key, q) <= r2){
            bool success = listInsertLast(list, n->value);
            if(!success){
                printf("Error while inserting value in list");
                exit(EXIT_FAILURE);
            }
        }

        if (/* n dans la liste */){
            // possible appel sur branche gauche;
            setListBst2d(bst2d, list, n->left, q, r2); 
        }
        setListBst2d(bst2d, list, n->right, q, r2);
    }
}