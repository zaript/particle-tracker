#include    "global.h"

// Global variables initialization.

Integer NNodes = 0, NCells = 0;
TNode2d * Nodes = NULL;
TCell2d * Cells = NULL;

Real *ux = NULL, *uy = NULL, *duxdx = NULL, *duydx = NULL, *duxdy = NULL, *duydy = NULL;

Integer NNeighbours = 0;
Integer ** Neighbours = NULL;

Integer CurrentCell = 0;

// Some global functions.

inline FILE *
xfopen(char *fname) {
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        fprintf ( stderr, "\nError while reading input file %s.\n", fname );
        exit (EXIT_FAILURE);
    }
    return f;
}

inline void *
xmalloc(unsigned n) {
    void *p;
    p = malloc(n);
    if ( p == NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    return p;
}

int 
allocateGlobals() {
    if ((NNodes > 0) && (NCells > 0) && (Nodes == NULL) && (Cells == NULL) && (ux == NULL) && (uy == NULL) && (duxdx == NULL) && (duxdy == NULL) && (duydx == NULL) && (duydy == NULL)) {
        Nodes = (TNode2d *)xmalloc ( sizeof(TNode2d) * NNodes );
        Cells = (TCell2d *)xmalloc ( sizeof(TCell2d) * NCells );
        ux = (Real *)xmalloc ( sizeof(Real) * NNodes );
        uy = (Real *)xmalloc ( sizeof(Real) * NNodes );
        duxdx = (Real *)xmalloc ( sizeof(Real) * NNodes );
        duxdy = (Real *)xmalloc ( sizeof(Real) * NNodes );
        duydx = (Real *)xmalloc ( sizeof(Real) * NNodes );
        duydy = (Real *)xmalloc ( sizeof(Real) * NNodes );
        return EXIT_SUCCESS;
    } else {
        printf("Memory for globals already allocated or NNodes(NCells) == 0.\n");
        return EXIT_FAILURE;
    }
}

int
freeMem() {
    free(Nodes);
    Nodes = NULL;
    free(Cells);
    Cells = NULL;
    free(Neighbours);
    Neighbours = NULL;
    free(ux);
    ux = NULL;
    free(uy);
    uy = NULL;
    free(duxdx);
    duxdx = NULL;
    free(duxdy);
    duxdy = NULL;
    free(duydx);
    duydx = NULL;
    free(duydy);
    duydy = NULL;
    return EXIT_SUCCESS;
}
