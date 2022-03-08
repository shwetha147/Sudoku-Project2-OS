#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>

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
            if (sudoku[i][j] < 1 || sudoku[i][j] > 9){
                flag = false;
                printf("Invalid number entered. Row: %d, Col: %d, value: %d\n", i, j, sudoku[i][j]);
                break;
            }
        }
    }
}


// checks all 9 rows to see if all rows are valid
void *validRows(void* params){
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
                pthread_exit(0
                );
            }
        }
    }
    validFlags[0] = 1;
    pthread_exit(0);
}

/* checks all 9 cols to see if all colss are valid */
void *validColumns(void* params){
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

/* checks all 9 square to see if all squares are valid */
void *validSquares(void* params){
    int temp;
    parameters *param = (parameters*) params;
    int cornerRow = param->row;
    int cornerCol = param->col;

    int flagSquare[9] = {0,0,0,0,0,0,0,0,0}; 
    for(int i = cornerRow; i < cornerRow+3; i++){
        for(int j = cornerCol; j< cornerCol + 3; j++){ /* goes through small square from left corner index */
            temp = sudoku[i][j]; /* pull individual value */
           /* printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]); */
 
            if(temp>0 && temp<10 && (flagSquare[temp-1] == 0)){ /* if doesnt exist, change flag to 1 */
                flagSquare[temp-1] = 1;
            }
            else{
                printf("INVALID SQUARE DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]);
                pthread_exit(0);
            }
        }
    }
    validFlags[2] = 1;
    pthread_exit(0);
}


int main() {
    /* Reading a sample file */ /* NEED TO ADD A USER INPUT HERE! */
	FILE *board = fopen("board.txt", "r");
	fileInput(board);

    /* Prints Sudoku Board */
    printf("Sudoku Board Inputted: \n");
    for(int i = 0; i < 9; i++){
       for(int j = 0; j < 9; j++){
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }


    /*int shm_fd;
    shm_fd = shm_open("VALID", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 4096);
    int *ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);*/

    // Option 1: 11 thread, 9 for each square, 1 for all rows and 1 for all columns
    pthread_t threads[11];
    pthread_create(&threads[0], NULL, validRows, NULL);
    pthread_create(&threads[1], NULL, validColumns, NULL);

    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if(i%3 == 0 && j%3==0){
                parameters *data = (parameters*) malloc(sizeof(parameters));
                data->row = i;
                data->col = j;
                pthread_create(&threads[j+2], NULL, validSquares, data);
            }
        }
    }

    for (int i = 0; i<11; i++){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < 3; i++){
        if(validFlags[i] == 0){
            printf("SUDOKU IS INVALID YOU DUMB ASS\n");
            return EXIT_SUCCESS;
        }
    }
    printf("yay");
    return EXIT_SUCCESS;

    return 0;
}
