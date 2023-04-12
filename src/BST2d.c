/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdbool.h>
#include <stdlib.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"

// A compléter
struct BST2d
{
    BNode *root;
    size_t size;

    // En créeant notre BST, on lui fournira la fonction de comparaison relative à ce BST qui permettra d'insérer les éléments
    int (*compfn)(void *, void *);
};