#include <stdio.h>
#include <math.h>

typedef double Real;
typedef int Integer;

typedef struct {
   Real x,y;
} TNode2d;

typedef struct{
   Integer type;
   Integer n1,n2,n3,n4;
} TCell2d;

Integer NNodes, NCells;
TNode2d * Nodes;
TCell2d * Cells;
Integer NNeighbours;
Integer ** Neighbours;
Real * ux,uy,duxdx,duydx,duxdy,duydy;
Integer CurrentCell;
