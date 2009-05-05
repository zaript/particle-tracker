#define real double
#define integer int
#include <stdio.h>
#include <math.h>

typedef struct {
   real x,y;
} TNode2d;

typedef struct{
   integer type;
   integer n1,n2,n3,n4;
} TCell2d;

integer NNodes, NCells;
TNode2d * Nodes;
TCell2d * Cells;
integer NNeighbours;
integer ** Neighbours;
real * ux,uy,duxdx,duydx,duxdy,duydy;
integer CurrentCell;
