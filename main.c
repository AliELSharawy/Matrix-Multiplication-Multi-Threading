#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

long matrix1[20][20];
long matrix2[20][20];

long resMatrix1[20][20];
long resMatrix2[20][20];
long resMatrix3[20][20];


struct arguments{
    int rowIndex;
    int colIndex;
    int col1;
    int col2;
};

void matrixThread(int row1, int col1, int col2, char* outFile1Name){

    long sum = 0;
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
                for(int k = 0; k < col1; k++){
                    sum = sum + matrix1[i][k] * matrix2[k][j];
                }
                resMatrix1[i][j] = sum;
                sum = 0;
        }
    }

    // print to file
    FILE *file1 = fopen(outFile1Name,"w");
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            fprintf(file1,"%ld\t",resMatrix1[i][j]);
        }
        fprintf(file1,"\n");
    }
}

void* rowThreadFill(void* arg){
    struct arguments *rowData;
    rowData = (struct arguments *) arg;

    long sum = 0;
    for(int i = 0; i < rowData -> col2; i++){
        for(int j = 0; j < rowData -> col1; j++){
          sum = sum + matrix1[rowData -> rowIndex][j] * matrix2[j][i];
        }
        resMatrix2[rowData -> rowIndex][i] = sum;
        sum = 0;
    }
    // free memory allocated for struct
    free(arg);
}

void rowThread(int row1, int col1,int col2, char* outFile2Name){

    pthread_t rowTreads[row1];

    for(int i = 0; i < row1; i++){
        // allocate struct to memory to be sent to row thread fill function
        struct arguments *argData = malloc(sizeof(struct arguments));
        argData -> rowIndex = i;
        argData -> col1 = col1;
        argData -> col2 = col2;
        int rc = pthread_create(&rowTreads[i],NULL,rowThreadFill,(void*) argData);
        if(rc){
            printf("Error, can't create threads");
            exit(-1);
        }
    }

    // join the threads & print to file
    FILE *file2 = fopen(outFile2Name,"w");
    for(int i = 0; i < row1; i++){
        pthread_join(rowTreads[i],NULL);
        for(int j = 0; j < col2; j++){
            fprintf(file2,"%ld\t",resMatrix2[i][j]);
        }
        fprintf(file2,"\n");
    }
}

void* elementThreadFill(void* arg){

    struct arguments *elementData;
    elementData = (struct arguments *) arg;

    long sum = 0;
    for(int i = 0; i < elementData -> col1; i++)
        sum = sum + matrix1[elementData ->rowIndex][i] * matrix2[i][elementData -> colIndex];
    resMatrix3[elementData -> rowIndex][elementData -> colIndex] = sum;
    sum = 0;
    // free memory allocated for struct
    free(arg);
}

void elementThread(int row1, int col1, int col2, char* outFile3Name){

    pthread_t elementThreads[row1][col2];

    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            struct arguments *argElementData = malloc(sizeof(struct arguments));
            argElementData -> rowIndex = i;
            argElementData -> colIndex = j;
            argElementData -> col1 = col1;
            int rc = pthread_create(&elementThreads[i][j], NULL, elementThreadFill, (void*) argElementData);
            if(rc){
                printf("Error, can't create threads");
                exit(-1);
            }
        }
    }

    FILE *file3 = fopen(outFile3Name,"w");
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            pthread_join(elementThreads[i][j],NULL);
            fprintf(file3,"%ld\t",resMatrix3[i][j]);
        }
        fprintf(file3,"\n");
    }
}

int main(int argc, char **argv)
{

    char file1Name[23];
    //scanf("%s",&file1Name);

    char file2Name[23];
    //scanf("%s",&file2Name);


    char file3Name[23];
    //scanf("%s",&file3Name);

    if(argc == 1){
        strcpy(file1Name,"a");
        strcpy(file2Name,"b");
        strcpy(file3Name,"c");
    }
    else if(argc == 4){
        strcpy(file1Name,argv[1]);
        strcpy(file2Name,argv[2]);
        strcpy(file3Name,argv[3]);
    }

    strcat(file1Name,".txt");
    strcat(file2Name,".txt");

    char outFile1Name[23];
    strcpy(outFile1Name,file3Name);
    strcat(outFile1Name,"_per_matrix.txt");

    char outFile2Name[23];
    strcpy(outFile2Name,file3Name);
    strcat(outFile2Name,"_per_row.txt");

    char outFile3Name[23];
    strcpy(outFile3Name,file3Name);
    strcat(outFile3Name,"_per_element.txt");

    // extract data from file 1 to get matrix 1
    FILE *file1 = fopen(file1Name,"a+");
    char dim1[6];
    fscanf(file1,"%s",&dim1);
    char row1[] = {dim1[4],dim1[5]};
    row1[2] = NULL;
    int row1Value = atoi(row1);

    fscanf(file1,"%s",&dim1);
    char col1[2] = {dim1[4],dim1[5]};
    col1[2] = NULL;
    int col1Value = atoi(col1);

    for(int i = 0; i < row1Value; i++){
        for(int j = 0; j < col1Value; j++)
            fscanf(file1,"%d",&matrix1[i][j]);
    }

    // extract data from file 1 to get matrix 1
    FILE *file2 = fopen(file2Name,"a+");
    char dim2[6];
    fscanf(file2,"%s",&dim2);
    char row2[] = {dim2[4],dim2[5]};
    row2[2] = NULL;
    int row2Value = atoi(row2);

    fscanf(file2,"%s",&dim2);
    char col2[2] = {dim2[4],dim2[5]};
    col2[2] = NULL;
    int col2Value = atoi(col2);

    for(int i = 0; i < row2Value; i++){
        for(int j = 0; j < col2Value; j++)
            fscanf(file2,"%d",&matrix2[i][j]);
    }

    if(col1Value != row2Value){
        printf("invalid col1 != row1");
        exit(0);
    }

    // execute function 1

    struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
    matrixThread(row1Value,col1Value,col2Value,outFile1Name);
    gettimeofday(&stop, NULL); //end checking time

    printf("number of threads used by function 1 = 1\n");
    printf("Seconds taken by function 1 (matrix thread) %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken by function 1 (matrix thread): %lu\n\n", stop.tv_usec - start.tv_usec);

    //execute function 2

    gettimeofday(&start, NULL); //start checking time
    rowThread(row1Value,col1Value,col2Value,outFile2Name);
    gettimeofday(&stop, NULL); //end checking time

    printf("number of threads used by function 2 = %d\n",row1Value);
    printf("Seconds taken by function 2 (row thread) %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken by function 2 (row thread): %lu\n\n", stop.tv_usec - start.tv_usec);

    //execute function 3

    gettimeofday(&start, NULL); //start checking time
    elementThread(row1Value,col1Value,col2Value,outFile3Name);
    gettimeofday(&stop, NULL); //end checking time

    printf("number of threads used by function 3 = %d\n",row1Value * col2Value);
    printf("Seconds taken by function 3 (element thread) %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken by function 3 (element thread): %lu\n", stop.tv_usec - start.tv_usec);

    return 0;
}
