 /* #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h> 
    #include <unisted.h> 
*/

/* sudoku board will be stored in following array */
int sudoku[9][9];

/* Method that reads inputted sudoku puzzle */
void fileInput(FILE *file){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }
}

/*  An array of int values is needed that will be visible to each thread created. 
    In this array, values of 0 or 1 are stored. Indicating whether worker thread no. is valid. 
    So, if the current thread has a valid sudoku board the value stored in the valid[n] will be 1. */
int valid[11];

/* Variable to keep track of 3x3 sub boards specifically */
int threeByThree = 2;

/* Structure for passing data to threads */ /** txtbk chapter 4 **/
typedef struct{
    int row;
    int col;
}parameters

/* Child Thread that checks Rows */
void *validRow(void *params){
    int i;
    int j;

    parameters * 

}
