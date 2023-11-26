#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MATRIX_SIZE 1000 // the matrix size could be modified

int matrix1[size][size]; // matrix1 of content1
int matrix2[size][size]; // matrix2 of content2

// a function to multiply subMatrices
void multiplyMatrices(int result[size][size], int matrix1[size][size], int matrix2[size][size], int startRow, int endRow)
{
    for (int i = startRow; i < endRow; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            result[i][j] = 0;
            for (int k = 0; k < size; ++k)
            {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// a function to print the matrix in a matrix format
void printMatrix(int matrix[size][size])
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
            printf((j == size - 1) ? "%d\n" : "%d ", matrix[i][j]);
    }
    printf("\n\n");
}

// main function
int main()
{
    // matrices generation ===================================================

    // variables---------------
    int content1[7] = {1, 2, 1, 1, 7, 5, 3};           // id = 1211753
    int content2[10] = {2, 4, 2, 7, 1, 4, 1, 2, 5, 9}; // id * year of birth = 1211753 * 2003 = 2427141259
    int counter = 0;                                   // a counter to iterate circularry around the contents
    //------------------------

    // matrix1-------------------------------------------------
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++, counter = (counter == 6) ? 0 : ++counter)
            matrix1[i][j] = content1[counter];
    counter = 0;
    // --------------------------------------------------------

    // matrix2 ------------------------------------------------
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++, counter = (counter == 9) ? 0 : ++counter)
            matrix2[i][j] = content2[counter];
    counter = 0;
    // --------------------------------------------------------

    // printf("matrix1: \n");
    // printMatrix(matrix1);
    // printf("matrix2: \n");
    // printMatrix(matrix2);

    // end of matrices generation ==============================================

    // matrix multiplication b) multiprocess approach-----------------------------------------
    struct timeval start_time2, end_time2;

    gettimeofday(&start_time2, NULL);

    key_t key = ftok("/tmp", 'A'); // my shared memory key
    int mId = shmget(key, sizeof(matrix1), IPC_CREAT | 0666);
    if (mId == -1)
    {
        printf("failed to create shared memory\n");
        return 1;
    }
    int(*sharedMatrix)[size] = shmat(mId, NULL, 0); // automatic address assigned NULL && 0

    int numOfProcesses = 10;

    // number of rows each process should handle is calculated by the matrix size divided by the number of processes
    int rowsPerProcess = size / numOfProcesses; // 100/10 = 10 (in this case)

    // processes creation using fork()
    for (int i = 0; i < numOfProcesses; ++i) // create 2^10 child processes
    {
        pid_t pId = fork(); // each process will hold an id or a pid_t

        if (pId == -1) // in case the process creation failed
        {
            printf("failed creating process\n");
            return 2;
        }

        if (pId == 0) // if the current process is a child process
        {
            // deciding the edges of each submatrix----
            int startRow = i * rowsPerProcess;
            int endRow = (i == numOfProcesses - 1) ? size : startRow + rowsPerProcess;
            //-----------------------------------------

            // executing each submatrix
            multiplyMatrices(sharedMatrix, matrix1, matrix2, startRow, endRow);
            // finishing the process
            exit(0);
        }
    }

    // when a parent finish it should not reach the return so it's children do not turn
    // into orphan processes
    for (int i = 0; i < numOfProcesses; ++i)
    {
        wait(NULL);
    }

    gettimeofday(&end_time2, NULL);

    // Print the result matrix
    printf("the result of the seconde method (multiprocessing)\n");
    printMatrix(sharedMatrix);

    double elapsed_time2 = (end_time2.tv_sec - start_time2.tv_sec) +
                           (end_time2.tv_usec - start_time2.tv_usec) / 1000000.0;
    // clean it up!
    shmdt(sharedMatrix);
    shmctl(mId, IPC_RMID, NULL);

    printf("Execution Time: %f seconds\n", elapsed_time2);

    // end of matrix multiplication b-----------------------------------------------------
    return 0;
}