#include "interpol.h"

int main()
{
   Real x,y;
   x=9; y=1;
   TNode2d n1,n2,n3,n4;
   n1.x=0; n1.y=0;
   n2.x=10; n2.y=0;
   n3.x=10; n3.y=10;
   n4.x=0; n4.y=10;
   printf("Program is working! %f\n", CartCoordToCurv(x,y,n1,n4,n3,n2));
   return 0;
}
