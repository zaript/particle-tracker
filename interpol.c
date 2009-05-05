#include "interpol.h"

char InTriangle(Real x,Real y,TNode2d n1,TNode2d n2,TNode2d n3)
{
   if ( (n2.x-n1.x)*(y-n1.y)-(n2.y-n1.y)*(x-n1.x) < 0)
      return 0;
   if ( (n3.x-n2.x)*(y-n2.y)-(n3.y-n2.y)*(x-n2.x) < 0)
      return 0; 
   if ( (n1.x-n3.x)*(y-n3.y)-(n1.y-n3.y)*(x-n3.x) < 0)
      return 0;
   return 1;
}

char InQuadrilateral(Real x,Real y,TNode2d n1,TNode2d n2, TNode2d n3, TNode2d n4)
{
   if ( (n2.x-n1.x)*(y-n1.y)-(n2.y-n1.y)*(x-n1.x) < 0)
      return 0;
   if ( (n3.x-n2.x)*(y-n2.y)-(n3.y-n2.y)*(x-n2.x) < 0)
      return 0; 
   if ( (n4.x-n3.x)*(y-n3.y)-(n4.y-n3.y)*(x-n3.x) < 0)
      return 0;
   if ( (n1.x-n4.x)*(y-n4.y)-(n1.y-n4.y)*(x-n4.x) < 0)
      return 0;      
   return 1;
}

char InCell2d(Real x,Real y, TCell2d c)
{
   switch (c.type)
   {
      case 1: return InTriangle(x,y,Nodes[c.n1],Nodes[c.n2],Nodes[c.n3]);
      case 2: return InQuadrilateral(x,y,Nodes[c.n1],Nodes[c.n2],Nodes[c.n3],Nodes[c.n4]);
   }
}

Real InterpolateInTriangle(Real x, Real y, TNode2d n1, TNode2d n2, TNode2d n3, Real f1, Real f2, Real f3)
{
   Real a1,a2,a3,det;
   det=(n2.x-n1.x)*(n3.y-n1.y)-(n3.x-n1.x)*(n2.y-n1.y);
   a2=((f2-f1)*(n3.y-n1.y)-(f3-f1)*(n2.y-n1.y))/det;
   a3=((n2.x-n1.x)*(f3-f1)-(n3.x-n1.x)*(f2-f1))/det;
   a1=f1-a2*n1.x-a3*n1.y;
   return a1+a2*x+a3*y;
}

Real SolveQE(Real a,Real b,Real c)
{
   Real x1,x2,d;
   d=b*b-4.0*a*c;
   if (d>=0)
   {
      x1=(-b+sqrt(d))/(2.0*a);
      x2=(-b-sqrt(d))/(2.0*a);
      if (x1>=-1.0 && x1<=1.0) return x1;
      if (x2>=-1.0 && x2<=1.0) return x2;
   }
}

Real CartCoordToCurv(Real x,Real y, TNode2d n1, TNode2d n2, TNode2d n3, TNode2d n4)
{
   Real ax,bx,cx,dx;
   Real ay,by,cy,dy;
   Real a,b,c;
   ax=(2.0*x-n1.x-n2.x)/2.0;
   bx=(n1.x-n2.x)/2.0;
   cx=(-n1.x-n2.x+n3.x+n4.x)/2.0;
   dx=(n1.x-n2.x+n3.x-n4.x)/2.0;
   ay=(2.0*y-n1.y-n2.y)/2.0;
   by=(n1.y-n2.y)/2.0;
   cy=(-n1.y-n2.y+n3.y+n4.y)/2.0;
   dy=(n1.y-n2.y+n3.y-n4.y)/2.0;
   a=bx*dy-dx*by;
   b=ax*dy+bx*cy-cx*by-dx*ay;
   c=ax*cy-cx*ay;
   if (fabs(a)>1e-05) 
      return SolveQE(a,b,c);
   else
      return -c/b;
}

Real InterpolateInQuadrilateral(Real x,Real y, TNode2d n1, TNode2d n2, TNode2d n3, TNode2d n4, Real f1, Real f2, Real f3, Real f4)
{
   Real a1,a2,a3,a4;
   Real u,v;
   u=CartCoordToCurv(x,y,n1,n2,n3,n4);
   v=CartCoordToCurv(x,y,n1,n4,n3,n2);
   a1=(1.0-u)*(1.0-v)/4.0;
   a2=(1.0+u)*(1.0-v)/4.0;
   a3=(1.0+u)*(1.0+v)/4.0;
   a4=(1.0-u)*(1.0+v)/4.0;
   return a1*f1+a2*f2+a3*f3+a4*f4;
}

Real Interpolate2d(Real x,Real y,TCell2d c,Real * q)
{
   switch (c.type)
   {
      case 1: return InterpolateInTriangle(x,y,Nodes[c.n1],Nodes[c.n2],Nodes[c.n3],q[c.n1],q[c.n2],q[c.n3]);
      case 2: return InterpolateInQuadrilateral(x,y,Nodes[c.n1],Nodes[c.n2],Nodes[c.n3],Nodes[c.n4],q[c.n1],q[c.n2],q[c.n3],q[c.n4]);
   }
}
