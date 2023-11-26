#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <pthread.h>
#define MATRIX_SIZE 100 // the matrix size could be modified
#define NUM_THREADS 6	// the number of threads can be modified

int matrix1[size][size];			   // matrix1 of content1
int matrix2[size][size];			   // matrix2 of content2
int resultMatrix3[size][size] = {0}; // matrix of multiplication3

struct // a struct to hold the threads data (where to start and where to stop )
{
	int sr;
	int er;
} typedef thread;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

// another function to multiply matrices but holds other parameters
void *matrixMult(void *param)
{
	t_data *data = (t_data *)param;

	for (int i = data->sr; i < data->er; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			resultMatrix3[i][j] = 0;
			for (int k = 0; k < size; ++k)
			{
				// pthread_mutex_lock(&mutex);
				resultMatrix3[i][j] += matrix1[i][k] * matrix2[k][j];
				// pthread_mutex_unlock(&mutex);
			}
		}
	}

	pthread_exit(NULL);
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
	int content1[7] = {1, 2, 1, 1, 7, 5, 3};		   // id = 1211753
	int content2[10] = {2, 4, 2, 7, 1, 4, 1, 2, 5, 9}; // id * year of birth = 1211753 * 2003 = 2427141259
	int counter = 0;								   // a counter to iterate circularry around the contents
	int resultMatrix1[size][size] = {0}; // matrix of multiplication1
	int resultMatrix2[size][size] = {0}; // matrix of multiplication2

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

	// end of matrices generation ==============================================

	// matrix multiplication a) naive approach-----------------------------------------
	clock_t start_time = clock();
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			resultMatrix1[i][j] = 0;
			for (int k = 0; k < size; ++k)
				resultMatrix1[i][j] += matrix1[i][k] * matrix2[k][j];
		}
	}
	// printf("matrix1: \n");
	// printMatrix(matrix1);
	// printf("matrix2: \n");
	// printMatrix(matrix2);

	printf("the result of the first method ( naive approach ):\n");
	printMatrix(resultMatrix1);
	clock_t end_time = clock();
	double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Execution Time: %f seconds\n", elapsed_time);
	// end of matrix multiplication a-----------------------------------------------------

	//	matrix multiplication b) multiple child processes running in parallel-----------------------------------------
	clock_t start_time2 = clock();
	// Create shared memory for the result matrix
	key_t key = ftok("/tmp", 'A'); // my shared memory key
	int mId = shmget(key, sizeof(resultMatrix2), IPC_CREAT | 0666);
	if (mId == -1)
	{
		printf("failed to create shared memory\n");
		return 1;
	}
	int(*sharedMatrix)[size] = shmat(mId, NULL, 0); // automatic address assigned NULL && 0

	int numOfProcesses = 20;

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
	// Print the result matrix
	printf("the result of the seconde method (muli processing)\n");
	printMatrix(sharedMatrix);

	// clean it up!
	shmdt(sharedMatrix);
	shmctl(mId, IPC_RMID, NULL);
	clock_t end_time2 = clock();
	double elapsed_time2 = ((double)(end_time2 - start_time2)) / CLOCKS_PER_SEC;
	printf("Execution Time: %f seconds\n", elapsed_time2);
	//  end of matrix multiplication b -------------------------------------------------------------------------------

	// matrix multiplication c) a multiple threads running in paralle------------------------------------------------
	clock_t start_time3 = clock();
	// threads creation
	pthread_t threads[NOT];
	t_data thread_data[NOT];

	// tasks dividing among the threads
	int rows_per_thread = size / NOT; // setting the shunk size each thread should hold
	int remaining_rows = size % NOT;	 // dealling with any number of threads
	int current_row = 0;							 // current row is 0 by default;

	for (int i = 0; i < NOT; i++)
	{
		// setting each threads' data
		thread_data[i].start_row = current_row;
		thread_data[i].end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
		current_row = thread_data[i].end_row;

		// lets get to work!!
		pthread_create(&threads[i], NULL, matrixMult, &thread_data[i]);
	}

	// waiting to finish all threads
	for (int i = 0; i < NOT; i++)
	{
		pthread_join(threads[i], NULL);
	}

	printMatrix(resultMatrix3);

	// time{
	clock_t end_time3 = clock();
	double elapsed_time3 = ((double)(end_time3 - start_time3)) / CLOCKS_PER_SEC;
	printf("Execution Time: %f seconds\n", elapsed_time3);
	//}

	//  end of matrix multiplication c-------------------------------------------------------------------------------

	return 0;
}

// finished 6:52 AM 11/12/2023
// MR.AbdulRahman shaheen
// error codes:
//
//
//
//
//