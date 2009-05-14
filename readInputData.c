/*#include "global.h"*/
#include	"readInputData.h"

#define LINE_MAX 128
#define NASTRANFieldLength 16
#define NASTRANPointNumberPosition 24
#define NASTRANValuePosition 40

int
setMarker (char *bufstr) {
    int marker = 0;
    if (strstr(bufstr, "Grid points") != NULL) {marker = 1;}     //points section
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
    Real tempz = 0, tempx = 0, tempy = 0;
    if (bufstr[0] == 'G') {
        sscanf(bufstr, "%*s %*d %*d %lg %lg", &tempx, &tempy);
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

    if (strstr(bufstr, "CTRIA3")) {
        sscanf(bufstr, "%*s %*d %*d %d %d %d", &cells[i].n1, &cells[i].n2, &cells[i].n3);
        cells[i].type = 1;
    }
    if (strstr(bufstr, "CQUAD4")) {
        sscanf(bufstr, "%*s %*d %*d %d %d %d %d", &cells[i].n1, &cells[i].n2, &cells[i].n3, &cells[i].n4);
        cells[i].type = 2;
    }
    return 1;
}

int
readValuesInNodes (char *bufstr,
        Real *valuesArray) {

    /*TODO: удалить этот грязный хак.*/
    /*int i, ii;*/
    /*Real tempValue;*/
    /*ii = sscanf(bufstr, "%*s %*d%4d%16lg", &i, &tempValue);*/
    /*printf(" ----\n %d tokens %d %.15f\n\n", ii, i, tempValue);*/
    /*valuesArray[i-1] = tempValue;*/
    char tempstr[LINE_MAX];
    strncpy(tempstr, &bufstr[NASTRANPointNumberPosition], NASTRANFieldLength);
    int i = atoi(tempstr) - 1;
    valuesArray[i] = atof(&bufstr[NASTRANValuePosition]);
    printf(" ----\n%d %.15f\n\n", i, valuesArray[i]);
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
    int marker = 0;
    char bufstr[LINE_MAX];
    printf("test\n");
    while ( fgets(bufstr, LINE_MAX, ifile) != NULL) {
        if (bufstr[0] == '$') {
            marker = setMarker(bufstr);
            continue;
        }
        if (bufstr[0] == 'E') {
            if (strstr(bufstr, "ENDDATA") != NULL) {continue;}
        }
        switch (marker) {
            case 1:
                if (bufstr[0] == 'G') { (*nodesNum)++; }
                break;
            case 2:
                (*cellsNum)++;
                break;
            default: break;
        }
    }
    fclose(ifile);

    /*// Allocate memory for data arrays.*/
    printf("Allocate memory for data arrays.\n");
    nodes = (TNode2d *)malloc ( sizeof(TNode2d) * (*nodesNum) );
    if ( nodes==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    cells = (TCell2d *)malloc ( sizeof(TCell2d) * (*cellsNum) );
    if ( cells==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    ux = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( ux==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    uy = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( uy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duxdx = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( duxdx==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duxdy = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( duxdy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duydx = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( duydx==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    duydy = (Real *)malloc ( sizeof(Real) * (*nodesNum) );
    if ( duydy==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }
    printf("test passed\n");

    // Actual read.
    int i = 0;
    marker = 0;
    ifile = fopen(ifile_name, "r");
    while ( fgets(bufstr, LINE_MAX, ifile) != NULL) {
        if (bufstr[0] == '$') {
            marker = setMarker(bufstr);
            printf("%s %d\n", bufstr, marker);
            /*int i = 0;*/
            continue;
        }
        if (bufstr[0] == 'E') {
            if (strstr(bufstr, "ENDDATA") != NULL) {continue;}
            printf("End of file.\n");
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
                /*printf("%s\n", bufstr);*/
                readValuesInNodes(bufstr, uy);
                break;
            case 5:
                /*printf("%s\n", bufstr);*/
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
