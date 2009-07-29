/* 
 * Read data from file in NASTRAN format.
 *
 * Algoritm:
 * 1) Read until line beginning with "$"
 * 2) Set section marker.
 * 3) Repeat until end of the file.
 */

#include    "global.h"

#define LINE_MAX 128
#define NASTRANFieldLength 16
#define NASTRANPointNumberPosition 24
#define NASTRANValuePosition 40

static int
setMarker (char *bufstr) {
    int marker = 0;
    if (strstr(bufstr, "Grid points") != NULL) {marker = 1;}     //points section
    if (strstr(bufstr, "Elements") != NULL) {marker = 2;}        //cells section
    if (strstr(bufstr, "Node scalar, Velocity X") != NULL) {marker = 3;}
    if (strstr(bufstr, "Node scalar, Velocity Y") != NULL) {marker = 4;}
    if (strstr(bufstr, "Node scalar, dx-velocity-dx") != NULL) {marker = 5;}
    if (strstr(bufstr, "Node scalar, dy-velocity-dx") != NULL) {marker = 6;}
    if (strstr(bufstr, "Node scalar, dx-velocity-dy") != NULL) {marker = 7;}
    if (strstr(bufstr, "Node scalar, dy-velocity-dy") != NULL) {marker = 8;}
    return marker;
}

static int 
countNodesAndCells(FILE *ifile) {
    NNodes = 0;
    NCells = 0;
    int marker = 0;
    char bufstr[LINE_MAX] = "";
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
                // file contain 2 lines for each node, so we count only one of them
                if (bufstr[0] == 'G') { NNodes++; }
                break;
            case 2:
                NCells++;
                break;
            default: break;
        }
    }
    return EXIT_SUCCESS;
}

static int
readNodeData (char *bufstr, 
    int i) {

    int success = 0;
    Real x = 0, y = 0;
    if (bufstr[0] == 'G') {
        sscanf(bufstr, "%*s %*d %*d %lg %lg", &x, &y);
        Nodes[i].x = x;
        Nodes[i].y = y;
        success = 1;
    }
    return success;
}

static int
readCellData (char *bufstr,
    int i) {

    if (strstr(bufstr, "CTRIA3")) {
        sscanf(bufstr, "%*s %*d %*d %d %d %d", &Cells[i].n1, &Cells[i].n2, &Cells[i].n3);
        Cells[i].type = 1;
    }
    if (strstr(bufstr, "CQUAD4")) {
        sscanf(bufstr, "%*s %*d %*d %d %d %d %d", &Cells[i].n1, &Cells[i].n2, &Cells[i].n3, &Cells[i].n4);
        Cells[i].type = 2;
    }
    return EXIT_SUCCESS;
}

static int
readValuesInNodes (char *bufstr,
    Real *valuesArray) {

    char tempstr[LINE_MAX] = "";
    strncpy(tempstr, &bufstr[NASTRANPointNumberPosition], NASTRANFieldLength);
    int i = atoi(tempstr) - 1;
    valuesArray[i] = atof(&bufstr[NASTRANValuePosition]);
    return EXIT_SUCCESS;
}

// Parse file in NASTRAN format.
int
readNASTRANData	(char *ifile_name) {

    FILE *ifile;
    ifile = xfopen(ifile_name);

    // Count number of nodes and cells.
    countNodesAndCells(ifile);
    /*printf("NNodes = %d, NCells = %d.\n", NNodes, NCells);*/
    fclose(ifile);

    // Allocate memory for data arrays.
    allocateGlobals();
    /*printf("Memory for global arrays is allocated.\n");*/

    // Actual read.
    int i = 0;
    int marker = 0;
    char bufstr[LINE_MAX] = "";
    ifile = fopen(ifile_name, "r");
    while ( fgets(bufstr, LINE_MAX, ifile) != NULL) {
        if (bufstr[0] == '$') {
            marker = setMarker(bufstr);
            /*printf("%s %d\n", bufstr, marker);*/
            i = 0;
            continue;
        }
        if (bufstr[0] == 'E') {
            if (strstr(bufstr, "ENDDATA") != NULL) {continue;}
            /*printf("End of file.\n");*/
        }
        switch (marker) {
            case 1:
                if (readNodeData(bufstr, i) == 1) {
                    i++;
                }
                break;
            case 2:
                readCellData(bufstr, i);
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
    return EXIT_SUCCESS;
}
