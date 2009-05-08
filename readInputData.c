#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

#define LINE_MAX 1024
#define NASTRANFieldLength 16
#define NASTRANPointNumberPosition 24
#define NASTRANValuePosition 40

int
setMarker (char *bufstr) {
    int marker = 0;
    if (strstr(bufstr, "Grid points ") != NULL) {marker = 1;}     //points section
    if (strstr(bufstr, "Elements") != NULL) {marker = 2;}         //cells section
    if (strstr(bufstr, "Node scalar, Velocity X") != NULL) {marker = 3;}
    if (strstr(bufstr, "Node scalar, Velocity Y") != NULL) {marker = 4;}
    if (strstr(bufstr, "Node scalar, dx-velocity-dx") != NULL) {marker = 5;}
    if (strstr(bufstr, "Node scalar, dy-velocity-dx") != NULL) {marker = 6;}
    if (strstr(bufstr, "Node scalar, dx-velocity-dy") != NULL) {marker = 7;}
    if (strstr(bufstr, "Node scalar, dy-velocity-dy") != NULL) {marker = 8;}
    return marker;
}

int
readNodeData (char *bufstr, 
        TNode2d *nodes,
        int i) {

    int success = 0;
    /*char tempstr[100];*/
    char *tempstr;
    int tempint;
    Real tempz, tempx, tempy;
    if (bufstr[0] == 'G') {
        sscanf(bufstr, "%s %d %f %f %f", &tempstr, &tempint, &tempz, &tempx, &tempy); 
        nodes[i].x = tempx;
        nodes[i].y = tempy;
        success = 1;
    }
    return success;
}

int
readCellData (char *bufstr,
        TCell2d *cells,
        int i) {

    /*char tempstr[100];*/
    char *tempstr;
    int tempint;
    if (strstr(bufstr, "CTRIA3")) {
        sscanf(bufstr, "%s %d %d %d %d %d", &tempstr, &tempint, &tempint, &cells[i].n1, &cells[i].n2, &cells[i].n3);
        cells[i].type = 1;
    }
    if (strstr(bufstr, "CQUAD4")) {
        sscanf(bufstr, "%s %d %d %d %d %d", &tempstr, &tempint, &tempint, &cells[i].n1, &cells[i].n2, &cells[i].n3, &cells[i].n4);
        cells[i].type = 2;
    }
    return 1;
}

int
readValuesInNodes (char *bufstr,
        Real *valuesArray) {

    char *tempstr;
    int tempint, i;
    Real tempValue;
    sscanf(bufstr, "%8s %16d %16d %16f", &tempstr, &tempint, &i, &tempValue);
    valuesArray[i-1] = tempValue;
    return 1;
}

// Parse file in NASTRAN format.
// Carefull: function allocate memory, but does not free it.
int
readNASTRANData	(char *ifile_name,
        TNode2d *nodes,
        int *nodesNum,
        TCell2d *cells,
        int *cellsNum,
        Real *ux,
        Real *uy,
        Real *duxdx,
        Real *duxdy,
        Real *duydx,
        Real *duydy) {

    FILE *ifile;

    ifile = fopen(ifile_name, "r");
    if (ifile == NULL) {
        printf("Error while reading input file %s", ifile_name);
        return 0;
    }

    // Count number of nodes and elements.
    int marker;
    char *bufstr;
    while ( !feof(ifile) ) {
        fgets(bufstr, LINE_MAX, ifile);
        if (bufstr[0] == '$') {
            marker = setMarker(bufstr);
            continue;
        } 
        if (bufstr[0] == 'E') {
            if (strstr(bufstr, "ENDDATA") != NULL) {return 1;}
        }
        switch (marker) {
            case 1:
                if (bufstr[0] == 'G') { nodesNum++; }
                break; 
            case 2:
                cellsNum++;
                break; 
            default: break;
        }
    }
    fclose(ifile);

    // Allocate memory for data arrays.
    nodes = (TNode2d *)malloc ( sizeof(TNode2d) * nodesNum );
    if ( nodes==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    cells = (TCell2d *)malloc ( sizeof(TCell2d) * cellsNum );
    if ( cells==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    ux	= (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( ux==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    uy = (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( uy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duxdx = (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( duxdx==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duxdy = (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( duxdy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duydx = (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( duydx==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duydy = (Real *)malloc ( sizeof(Real) * nodesNum );
    if ( duydy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }

    // Actual read.
    int i = 0;
    ifile = fopen(ifile_name, "r");
    while ( !feof(ifile) ) {
        fgets(bufstr, LINE_MAX, ifile);
        if (bufstr[0] == '$') {
            marker = setMarker(bufstr);
            i = 0;
            continue;
        } 
        if (bufstr[0] == 'E') {
            if (strstr(bufstr, "ENDDATA") != NULL) {return 1;}
        }
        switch (marker) {
            case 1:
                if (readNodeData(bufstr, nodes, i) == 1) {
                    i++;
                }
                break; 
            case 2:
                readCellData(bufstr, cells, i);
                i++;
                break; 
            case 3:	
                readValuesInNodes(bufstr, ux);
                break; 
            case 4:
                readValuesInNodes(bufstr, uy);
                break; 
            case 5:
                readValuesInNodes(bufstr, duxdx);
                break; 
            case 6:
                readValuesInNodes(bufstr, duydx);
                break; 
            case 7:
                readValuesInNodes(bufstr, duxdy);
                break; 
            case 8:
                readValuesInNodes(bufstr, duydy);
                break; 
            default: break;
        }
    }
    fclose(ifile);
    return 1;
}
