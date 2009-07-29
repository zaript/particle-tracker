#include    "global.h"
#include	"readInputData.h"

int
main ( int argc, char *argv[] ) {

    if (argc > 1) {
        char *ifile_name;
        ifile_name = argv[1];
        printf("%s\n", ifile_name);
        
        readNASTRANData ( ifile_name );

        printf("%d %d\n", NNodes, NCells);
        printf("%f %f %f %f %f\n", ux[NNodes-1], uy[NNodes-1], duxdx[NNodes-1], duxdy[NNodes-1], duydx[NNodes-1]);
        printf("good\n");
    } else {
        printf("Define input file name.\n");
    }
    return EXIT_SUCCESS;
}
