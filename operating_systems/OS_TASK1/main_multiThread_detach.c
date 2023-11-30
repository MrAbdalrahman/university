#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define MATRIX_SIZE 100 // the matrix size could be modified
#define NUM_THREADS 4   // the number of threads can be modified

int matrix1[MATRIX_SIZE][MATRIX_SIZE];             // matrix1 of content1
int matrix2[MATRIX_SIZE][MATRIX_SIZE];             // matrix2 of content2
int resultMatrix3[MATRIX_SIZE][MATRIX_SIZE] = {0}; // matrix of multiplication3

struct // a struct to hold the threads data (where to start and where to stop )
{
    int start_row;
    int end_row;
} typedef t_data;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//  function to multiply matrices
void *multiply(void *param)
{
    t_data *data = (t_data *)param;

    for (int i = data->start_row; i < data->end_row; ++i)
    {
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
            resultMatrix3[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; ++k)
            {

                resultMatrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

// a function to print the matrix in a matrix format
void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
    {
        for (int j = 0; j < MATRIX_SIZE; ++j)
            printf((j == MATRIX_SIZE - 1) ? "%d\n" : "%d ", matrix[i][j]);
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
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++, counter = (counter == 6) ? 0 : ++counter)
            matrix1[i][j] = content1[counter];
    counter = 0;
    // --------------------------------------------------------

    // matrix2 ------------------------------------------------
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++, counter = (counter == 9) ? 0 : ++counter)
            matrix2[i][j] = content2[counter];
    counter = 0;
    // --------------------------------------------------------

    // printf("matrix1: \n");
    // printMatrix(matrix1);
    // printf("matrix2: \n");
    // printMatrix(matrix2);

    // end of matrices generation ==============================================

    // matrix multiplication c) a multiple threads running in paralle------------------------------------------------
    struct timeval start_time2, end_time2;

    gettimeofday(&start_time2, NULL);
    // threads creation

    pthread_t threads[NUM_THREADS];
    t_data thread_data[NUM_THREADS];
    pthread_attr_t detachedThread;
    pthread_attr_init(&detachedThread);
    pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);

    // tasks dividing among the threads
    int rows_per_thread = MATRIX_SIZE / NUM_THREADS; // setting the shunk size each thread should hold
    int remaining_rows = MATRIX_SIZE % NUM_THREADS;  // dealling with any number of threads
    int current_row = 0;                             // current row is 0 by default;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        // setting each threads' data
        thread_data[i].start_row = current_row;
        thread_data[i].end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
        current_row = thread_data[i].end_row;

        // lets get to work!!
        pthread_create(&threads[i], NULL, multiply, &thread_data[i]);
    }
    pthread_attr_destroy(&detachedThread);

    gettimeofday(&end_time2, NULL);
    printMatrix(resultMatrix3);

    double elapsed_time2 = (end_time2.tv_sec - start_time2.tv_sec) +
                           (end_time2.tv_usec - start_time2.tv_usec) / 1000000.0;

    printf("Execution Time: %f seconds\n", elapsed_time2);

    pthread_exit(0);
    //  end of matrix multiplication c-------------------------------------------------------------------------------
}