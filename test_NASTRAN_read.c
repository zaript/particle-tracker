/*
 * =====================================================================================
 *
 *       Filename:  test_NASTRAN_read.c
 *
 *    Description:  test module for reading from NASTRAN files.
 *
 *        Version:  1.0
 *        Created:  05/04/2009 01:54:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Timur Zaripov (), zaript@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

/*#include	<stdlib.h>*/
/*#include    "global.h"*/
#include	"readInputData.h"

int
main ( int argc, char *argv[] ) {
	char *ifile_name;
    TNode2d *nodes = NULL;
    int nodesNum = 0;
    TCell2d *cells = NULL;
    int cellsNum = 0;
    Real *ux = NULL;
    Real *uy = NULL;
    Real *duxdx = NULL;
    Real *duxdy = NULL;
    Real *duydx = NULL;
    Real *duydy = NULL;

    ifile_name = argv[1];
    printf("%s\n", ifile_name);
    readNASTRANData ( ifile_name, nodes, &nodesNum, cells, &cellsNum, ux, uy, duxdx, duxdy, duydx, duydy);
    printf("%d %d\n", nodesNum, cellsNum);
    printf("good\n");

    free(nodes);
    free(cells);
    free(ux);
    free(uy);
    free(duxdx);
    free(duxdy);
    free(duydx);
    free(duydy);
    return EXIT_SUCCESS;
}
