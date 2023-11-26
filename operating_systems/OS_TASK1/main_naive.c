#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MATRIX_SIZE 1000 // the matrix size could be modified

int matrix1[size][size]; // matrix1 of content1
int matrix2[size][size]; // matrix2 of content2

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
    int resultMatrix1[size][size] = {0}; // matrix of multiplication1
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
    // if you want to print the matrices 1,2 just uncomment these lines
    // printf("matrix1: \n");
    // printMatrix(matrix1);
    // printf("matrix2: \n");
    // printMatrix(matrix2);

    // end of matrices generation ==============================================

    // matrix multiplication a) naive approach-----------------------------------------
    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            resultMatrix1[i][j] = 0;
            for (int k = 0; k < size; ++k)
                resultMatrix1[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }

    printf("the result of the first method ( naive approach ):\n");
    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printMatrix(resultMatrix1);
    printf("Execution Time: %f seconds\n", elapsed_time);
    // end of matrix multiplication a-----------------------------------------------------
    return 0;
}