#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef double Real;
typedef int Integer;

typedef struct {
   Real x, y;
} TNode2d;

typedef struct{
   Integer type;
   Integer n1, n2, n3, n4;
} TCell2d;

// Initial data.
extern Integer NNodes, NCells;
extern TNode2d * Nodes;
extern TCell2d * Cells;

extern Real *ux, *uy, *duxdx, *duydx, *duxdy, *duydy;

// Self prodused data.
extern Integer NNeighbours;
extern Integer ** Neighbours;

extern Integer CurrentCell;

// Functions.
inline FILE *xfopen(char *);
inline void *xmalloc(unsigned);
extern int allocateGlobals();
extern int freeGlobals();
#endif /* GLOBAL_H */
