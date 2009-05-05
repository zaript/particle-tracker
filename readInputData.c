#include <stdio.h>
#include <string.h>
#include <limits.h>

int NASTRANFieldLength = 16;
int NASTRANPointNumberPosition = 24;
int NASTRANValuePosition = 40;

int
setMarker (
	char *bufstr
	) {
	marker = 0;
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
readNodeData (
	char *bufstr, 
	TNode2d *nodes,
    int i
	) {
    int success = 0;
	if (bufstr[0] = 'G') {
		sscanf(bufstr, "%s %d %f %f %f", &tempstr, &i, &z, &nodes[i].x, &nodes[i].y);
        success = 1;
	}
	return success;
}

int
readCellData (
	char *bufstr,
	TCell2d *cells,
    int i
	) {

	if (strstr(bufstr, "CTRIA3")) {
		sscanf(bufstr, "%s %d %d %d %d %d", &tempstr, &i, &tempint, &cells[i].n1, &cells[i].n2, cells[i].n3);
		cells[sell_i].type = 1;
	}
	if (strstr(bufstr, "CQUAD4")) {
		sscanf(bufstr, "%s %d %d %d %d %d", &tempstr, &i, &tempint, &cells[i].n1, &cells[i].n2, cells[i].n3, cells[i].n4);
		cells[sell_i].type = 2;
	}
	return 1;
}

int
readUglyPart
	(char *str,
	Real *valuesArray,
    int i
	) {

	strncopy(tempstr, bufstr[NASTRANPointNumberPosition], NASTRANFieldLength);
	int i = atoi(tempstr);
	valuesArray[i] = atof(bufstr[NASTRANValuePosition]);
	return 1;
}

int
countElementsNASTRAN (
	char *ifile_name
	) {
	FILE *ifile;
	// Count number of nodes and elements.

	ifile = fopen(ifile_name, "r");
	if (ifile = NULL) {
		printf("Error while reading input file %s", ifile_name);
		return 0;
	}

	char bufstr[LINE_MAX - 1];
	int marker = 0;
	bufstr[0] = '1';
	while ( !feof(ifile) ) {
		fgets(bufstr, LINE_MAX, ifile);
		if (bufstr[0] = '$') {
			marker = setMarker(bufstr);
			continue;
		} 
		if (bufstr[0] = 'E') {
			if (strstr(bufstr, "ENDDATA") != NULL) {return 1;}
		}
		switch (marker) {
			case 1:
				countNodes(bufstr, nodesCounter);
				break; 
			case 2:
				countCells(bufstr, cellsCounter);
				break; 
			default: break;
		}
	}
	fclose(ifile);
	return 1;
}

// Parse file in NASTRAN format.
// Carefull: function allocate memory, but does not free it.
int
readNASTRANData	(
	char *ifile_name
    ) {

	FILE *ifile;

	ifile = fopen(ifile_name, "r");
	if (ifile = NULL) {
		printf("Error while reading input file %s", ifile_name);
		return 0;
	}

	// Count number of nodes and elements.
    int nodesNum = 0;
    int cellsNum = 0;
	while ( !feof(ifile) ) {
		fgets(bufstr, LINE_MAX, ifile);
		if (bufstr[0] = '$') {
			marker = setMarker(bufstr);
			continue;
		} 
		if (bufstr[0] = 'E') {
			if (strstr(bufstr, "ENDDATA") != NULL) {return 1;}
		}
		switch (marker) {
			case 1:
	            if (bufstr[0] = 'G') { nodesNum++ }
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
    cells	= (TCell2d *)malloc ( sizeof(TCell2d) * cellsNum );
    if ( cells==NULL ) {
        fprintf ( stderr, "\ndynamic memory allocation failed\n" );
        exit (EXIT_FAILURE);
    }

	// Actual read.
	char bufstr[LINE_MAX - 1];
	int marker = 0;
    int i = 0;
	/*bufstr[0] = '1';*/
	ifile = fopen(ifile_name, "r");
	while ( !feof(ifile) ) {
		fgets(bufstr, LINE_MAX, ifile);
		if (bufstr[0] = '$') {
			marker = setMarker(bufstr);
            i = 0;
			continue;
		} 
		if (bufstr[0] = 'E') {
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
				readUglyPart(bufstr, ux, i);
                i++;
				break; 
			case 4:
				readUglyPart(bufstr, uy, i);
                i++;
				break; 
			case 5:
				readUglyPart(bufstr, duxdx, i);
                i++;
				break; 
			case 6:
				readUglyPart(bufstr, duydx, i);
                i++;
				break; 
			case 7:
				readUglyPart(bufstr, duxdy, i);
                i++;
				break; 
			case 8:
				readUglyPart(bufstr, duydy, i);
                i++;
				break; 
			default: break;
		}
	}
	fclose(ifile);
	return 1;
}
