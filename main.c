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
int validFlag = 1; /* assuming valid = 1 */

/* Structure for passing data to threads */
typedef struct parameters{
	int row;
    int col;
}parameters;

typedef struct {
	int value;
}oneVariable;

// parameters *data = (parameters*) malloc(sizeof(parameters));

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
               /*  printf("INVALID ROW DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]); */
                validFlag = 0;
                pthread_exit(0);
            }
        }
    }
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
               /*  printf("INVALID COLUMN DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n",j, i, sudoku[j][i]); */
                validFlag = 0;
                pthread_exit(0);
            }
        }
    }
    pthread_exit(0);
}
/* checks 1 col to see if col are valid */
void *validCol(void* params){
    oneVariable *param = (oneVariable*) params;
    int column = param->value;
    for(int i = 0; i < 9; i++){
        /* Reset flags for each row */
        int flagCol[9] = {0,0,0,0,0,0,0,0,0}; 
        if(flagCol[sudoku[i][column]-1] == 0){
            flagCol[sudoku[i][column]-1] = 1;
        }
        else{
            printf("INVALID COLUMN DETECTED. \n"); 
            printf("Row: %d, Col: %d, Value: %d\n",i, column, sudoku[i][column]); 
            validFlag = 0;
            pthread_exit(0);
        }
    }
    pthread_exit(0);
}

/* checks 1 col to see if col are valid */
void *validR(void* params){
    oneVariable *param = (oneVariable*) params;
    int r = param->value;
    for(int i = 0; i < 9; i++){
        /* Reset flags for each row */
        int flagCol[9] = {0,0,0,0,0,0,0,0,0}; 
        if(flagCol[sudoku[r][i]-1] == 0){
            flagCol[sudoku[r][i]-1] = 1;
        }
        else{
            /*  printf("INVALID COLUMN DETECTED. \n");
            printf("Row: %d, Col: %d, Value: %d\n",j, i, sudoku[j][i]); */
            validFlag = 0;
            pthread_exit(0);
        }
    }
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
               /*  printf("INVALID SQUARE DETECTED. \n");
                printf("Row: %d, Col: %d, Value: %d\n", i, j, sudoku[i][j]); */
                validFlag = 0;
                pthread_exit(0);
            }
        }
    }
    pthread_exit(0);
}

int validColsProcess(){ /* checks validity of all columns in process */
    
    printf("\ninside cols process\n");
    fflush(stdout);
    for(int i = 0; i < 9; i++){
        /* Reset flags for each col */
         printf("\ninside col %d\n", i);
         fflush(stdout);
        int flagCol[9] = {0,0,0,0,0,0,0,0,0}; 
        for(int j = 0; j< 9; j++){
            if(flagCol[sudoku[j][i]-1] == 0){
                flagCol[sudoku[j][i]-1] = 1;
                printf("\nflag true\n");
                fflush(stdout);
            } else{
                printf("\nflag FALSE\n");
                fflush(stdout);
               return 0;
            }
        }
    }
    return 1;
}

int validRowsProcess(){ /* checks validity of all rows in process */
    printf("\ninside EOWs process\n");
    fflush(stdout);
    for(int i = 0; i < 9; i++){
        /* Reset flags for each row */
        printf("\ninside ROW %d", i);
         fflush(stdout);
        int flagRow[9] = {0,0,0,0,0,0,0,0,0}; 
        for(int j = 0; j< 9; j++){
            if(flagRow[sudoku[i][j]-1] == 0){
                flagRow[sudoku[i][j]-1] = 1;
                 printf("\nrow flag true\n");
                fflush(stdout);
            }
            else{
               
                printf("\ntow flag false\n");
                fflush(stdout);
                return 0;
            }
        }
    }
    return 1;
}

/* checks all 9 square to see if all squares are valid */
int validSquaresProcess(){
    int temp = 0;

    int flagSquare[9] = {0,0,0,0,0,0,0,0,0}; 

    for(int i = 0; i<9; i++){ /* 9 times */
        for(int j = 0; j<9; j++){
             temp = sudoku[i][j]; 
                if(i%3 == 0 && j%3==0){ /* 9 times */
                    if(temp>0 && temp<10 && (flagSquare[temp-1] == 0)){ /* if doesnt exist, change flag to 1 */
                    flagSquare[temp-1] = 1;
                   /*  printf("valid square %d\n", i);
                    fflush(stdout); */
                }
                else{
                    /* printf("invalid square %d\n", i);
                    fflush(stdout); */
                    return 0;
                }
            }
        }
    }
    return 1; 
}

int main(int argc, char** argv) {
    /* Reading a sample file */ /* NEED TO ADD A USER INPUT HERE! */
	FILE *board = fopen("input.txt", "r");
	fileInput(board);
 printf("funny");
 fflush(stdout);
    int input = atoi(argv[1]);
    /* Prints Sudoku Board */
    printf("Sudoku Board Inputted: \n");
    for(int i = 0; i < 9; i++){
       for(int j = 0; j < 9; j++){
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
   
    printf("fun");
    fflush(stdout);
    int nThreads = 0;
    printf("%d hehe\n",nThreads);
    fflush(stdout);
    switch(input){
        case 1:
            nThreads = 11;
            break;
        case 2:
            nThreads = 27;
            break;
        case 3:
            nThreads = 0;
            break;
        default:
            nThreads = 0;
    }

    printf("%d\n",nThreads);
    fflush(stdout);
    /* for the threads cases */
    pthread_t threads[nThreads];
    int thread_num = 0;
    // parameters *data = (parameters*) malloc(9*sizeof(parameters));

    /* for the process case */
    int shm_fd;
    shm_fd = shm_open("VALID", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 4096);
    int *ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int result = 0;
    
    /* clock things */
    clock_t t;
    t = clock();
    printf("clock has been started: %ld\n", t);
    fflush(stdout);

    
    switch(input){
        case 1:
            /* Option 1: 11 thread, 9 for each square, 1 for all rows and 1 for all columns */
            printf("depression\n");
            fflush(stdout);
            struct parameters (*arr[9]) = {NULL};
            for(int i = 0; i<9; i++){
                arr[i] = (parameters*)malloc(sizeof(parameters)); 
            }
            // struct parameters *(*data) [] = &arr;
            pthread_create(&threads[thread_num], NULL, validRows, NULL);
            pthread_create(&threads[++thread_num], NULL, validColumns, NULL);
            
            int temp=0;
            for(int i = 0; i<9; i++){ /* 9 times */
                for(int j = 0; j<9; j++){
                    if(i%3 == 0 && j%3==0){ /* 9 times */
                        
                        arr[temp]->row = i;
                        arr[temp]->col = j;
                       
                        pthread_create(&threads[++thread_num], NULL, validSquares, arr[temp]);
                        printf("%d\n", thread_num);
                        fflush(stdout);
                        temp++;
                    }
                }
            }
            printf("made it");
            fflush(stdout);
            // free (data);
            break;

        case 2:
            /* Option 2: 27 threads, 1 per square, 1 per row and 1 per column */
            thread_num = 0;
            for(int i = 0; i<9; i++){
                for(int j = 0; j<9; j++){
                    if(i%3 == 0 && j%3==0){
                        parameters *data = (parameters*) malloc(sizeof(parameters));
                        data->row = i;
                        data->col = j;
                        pthread_create(&threads[thread_num++], NULL, validSquares, data);
                    }
                }
                oneVariable *data2 = (oneVariable*) malloc(sizeof(oneVariable));
                data2->value = i;
                pthread_create(&threads[thread_num++], NULL, validR, data2);
                pthread_create(&threads[thread_num++], NULL, validCol, data2);
            }
            break;
        case 3:
            /* Option 3: 1 process for all rows, 1 process for all columns, 1 process per square */
            thread_num = 0;
            printf("made it case 3");
            fflush(stdout);
            pid_t rows = fork();
            if (rows == 0){
                printf("made it jhgjgjg");
                fflush(stdout);
                result = validRowsProcess();
                if(result != 1){
                    /* valid columns */
                    *ptr = result;
                    printf("made it wooooo");
                    fflush(stdout);
                } 
                _Exit(EXIT_SUCCESS);
            } else {
                int waitFlag;
                waitpid(rows, &waitFlag, 0);
            }

            pid_t columns = fork();

            if (columns == 0){
                result = validColsProcess();
                if(result == 1){
                    /* valid columns */
                    *ptr = result;
                    printf("YAH BABY\n");
                     fflush(stdout);

                } 
                _Exit(EXIT_SUCCESS);
            }else{  
                printf("OHHH NOOOOO\n");
                int waitFlag;
                waitpid(columns, &waitFlag, 0);
            }
            
                
            pid_t square = fork();
            if(square == 0 ){
                result = validSquaresProcess();
            
                if(result == 1){
                    /* valid square */
                    printf("AVLID BABY");
                    fflush(stdout);
                    *ptr = result;
                    printf("resullt: %d", result);
                     fflush(stdout);
                } 
                _Exit(EXIT_SUCCESS);
            }else {
                int waitFlag;
                waitpid(square, &waitFlag, 0);
            }
        validFlag = *ptr;
        break;
    }

    printf("woooN\n");
            fflush(stdout);
    // validFlag = *ptr;
    shm_unlink("VALID");
    printf("first time t: %ld\n", t);
                    fflush(stdout);
    clock_t clk = clock();
     printf("time clock: %ld\n", clk);
                    fflush(stdout);
    t = clk - t;
    printf("time t: %ld\n", t);
                    fflush(stdout);
    double time_tot = ((double)t/CLOCKS_PER_SEC);


    for (int i = 0; i<nThreads; i++){
        printf("thread %d\n", i);
            fflush(stdout);
        pthread_join(threads[i], NULL);
    }
    printf("almost there");
    fflush(stdout);
    
    if(validFlag == 0 ){ /* valid when flag = 1 */
        printf("SOLUTION: NO (%f seconds)\n", time_tot);
        fflush(stdout);
    } else{
        printf("SOLUTION: YES (%f seconds)\n", time_tot);
        fflush(stdout);

    }
       
    // free(data);
    // free(data2);
    return 0;
}