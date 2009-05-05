#include "global.h"

char InTriangle(Real,Real,TNode2d,TNode2d,TNode2d);
char InQuadrilateral(Real,Real,TNode2d,TNode2d,TNode2d,TNode2d);
char InCell2d(Real,Real,TCell2d);
Real InterpolateInTriangle(Real,Real,TNode2d,TNode2d,TNode2d,Real,Real,Real);
Real SolveQE(Real,Real,Real);
Real CartCoordToCurv(Real,Real,TNode2d,TNode2d,TNode2d,TNode2d);
Real InterpolateInQuadrilateral(Real,Real,TNode2d,TNode2d,TNode2d,TNode2d,Real,Real,Real,Real);
Real Interpolate2d(Real,Real,TCell2d,Real*);
