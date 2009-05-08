/*
 * =====================================================================================
 *
 *       Filename:  global.c
 *
 *    Description: Some functions for global variables. 
 *
 *        Version:  1.0
 *        Created:  05/06/2009 10:27:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Timur Zaripov (), zaript@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

int
freeMem() {
    free(Nodes);
    free(Cells);
    free(Neighbours);
    free(ux);
    free(uy);
    free(duxdx);
    free(duxdy);
    free(duydx);
    free(duydy);
}
