#include "global.h"

char InTriangle(real,real,TNode2d,TNode2d,TNode2d);
char InQuadrilateral(real,real,TNode2d,TNode2d,TNode2d,TNode2d);
char InCell2d(real,real,TCell2d);
real InterpolateInTriangle(real,real,TNode2d,TNode2d,TNode2d,real,real,real);
real SolveQE(real,real,real);
real CartCoordToCurv(real,real,TNode2d,TNode2d,TNode2d,TNode2d);
real InterpolateInQuadrilateral(real,real,TNode2d,TNode2d,TNode2d,TNode2d,real,real,real,real);
real Interpolate2d(real,real,TCell2d,real*);
