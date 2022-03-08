#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Sudoku board will be stored in following array */
int sudoku[9][9];

/* Flags that are used to determine if row, column and boxes are valid */
int validFlags[3] = {0,0,0};

/* Structure for passing data to threads */
typedef struct {
	int row;
    int col;
}parameters;

/* Method that reads inputted sudoku puzzle */
void fileInput(FILE *file){
    bool flag = true;
    int i = 0;
    for(int i = 0; i < 9 && flag; i++){
        for(int j = 0; j < 9 && flag; j++){
            fscanf(file, "%d", &sudoku[i][j]);
            /* Ensuring Valid boards are given */
            /*if (sudoku[i][j] < 1 || sudoku[i][j] > 9){
                flag = false;
                printf("Invalid number entered. Row: %d, Col: %d, value: %d\n", i, j, sudoku[i][j]);
                break;
            }*/
        }
    }
}



void *validRow(void* params){
    for(int i = 0; i < 9; i++){
        /* Reset flags for each row */
        int flagRow[9] = {0,0,0,0,0,0,0,0,0}; 
        for(int j = 0; j < 9; j++){
            /*  ex. if sudoku[i][j] == 5, 
                then it flagRow goes to 1 less than 5 and 
                changes the flag to 1.  */
            if(flagRow[sudoku[i][j]-1] == 0){
                flagRow[sudoku[i][j]-1] = 1;
            }
            else{
                printf("INVALID ROW DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]);
                pthread_exit(0);
            }
        }
    }
    validFlags[0] = 1;
    pthread_exit(0);
}

void *validColumn(void* params){
    for(int i = 0; i < 9; i++){
        /* Reset flags for each row */
        int flagCol[9] = {0,0,0,0,0,0,0,0,0}; 
        for(int j = 0; j< 9; j++){
            /*  ex. if sudoku[i][j] == 5, 
                then it flagCol goes to 1 less than 5 and 
                changes the flag to 1.  */
            if(flagCol[sudoku[j][i]-1] == 0){
                flagCol[sudoku[j][i]-1] = 1;
            }
            else{
                printf("INVALID COLUMN DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]);
                pthread_exit(0);
            }
        }
    }
    validFlags[1] = 1;
    pthread_exit(0);
}


int main() {
    /* Reading a sample file */ /* NEED TO ADD A USER INPUT HERE! */
	FILE *board = fopen("puzzle.txt", "r");
	fileInput(board);

    /* Prints Sudoku Board */
    printf("Sudoku Board Inputted: \n");
    for(int i = 0; i < 9; i++){
       for(int j = 0; j < 9; j++){
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

}
