#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>
#include <iomanip>
#include <time.h> 

using namespace std;

struct matrixMultiplicationProps{

	int randomSize;
	int* numbers = NULL;
	int rowEvaluate = 0;
	
};

void display(void* args)
{

	// Creat the struct
	struct matrixMultiplicationProps *matPropsDisp = (struct matrixMultiplicationProps*)args;

	int count = 0;
	int mat = 0;
	// Displaying matA	
    	cout << "2-D Matrix A:" << endl << endl;
	cout << setw(5);
   	for (int i = 0; i < matPropsDisp->randomSize*matPropsDisp->randomSize ; i++)
	{
		cout << matPropsDisp->numbers[i] << setw(5);
		count++;
		mat++;
		if (mat == matPropsDisp->randomSize*matPropsDisp->randomSize)
		{
			cout << endl;
			mat = 0;
		}
		if (count == matPropsDisp->randomSize)
		{
			cout << endl;
			count = 0;
		}
	}
}


// Normal 2d matrix transformation
void* transformDiagNorm(void* args)
{

	struct matrixMultiplicationProps *matTemp = (struct matrixMultiplicationProps*)args;

	int temp;
	int row = 2;
	int multi = 1;
	int i = 1;

	for(i; i < matTemp->randomSize*matTemp->randomSize; i++)
	{
		// Do transposition.
		temp = matTemp->numbers[i];
		matTemp->numbers[i] = matTemp->numbers[i+multi*(matTemp->randomSize-1)];
		matTemp->numbers[i+multi*(matTemp->randomSize-1)] = temp;
		multi++;
		
		// If the end of the row is reached. 7, 15, 23
		if (i == (row - 1)*(matTemp->randomSize)-1)
		{	
			i += row; // Start at a new i which is offset to start at next row.
			row++;
			multi = 1; // Reset multiplier to get correct values to transpose.
		}
		//cin.get();

	}
	return (NULL);
}

void* transformDiagPthread(void* args)
{
	matrixMultiplicationProps *matTemp = (struct matrixMultiplicationProps*)args;

	int temp;
	int multi = 1;
	int i = ((matTemp->rowEvaluate-1)*(matTemp->randomSize))+matTemp->rowEvaluate;

	for(i; i < (matTemp->rowEvaluate)*(matTemp->randomSize); i++)
	{
		// Do transposition.
		temp = matTemp->numbers[i];
		matTemp->numbers[i] = matTemp->numbers[i+multi*(matTemp->randomSize-1)];
		matTemp->numbers[i+multi*(matTemp->randomSize-1)] = temp;
		multi++;
	}
	return (NULL);
	
}

int main ( int argc, char* argv[] )
{
	// Define clock
	clock_t t;
	int randomSize = 8;

	// User input
	/* while (randomSize != 10 | randomSize != 20 | randomSize != 30)
	{
		cout << "Please enter either, 10, 20 or 30 for the size of the matrix you want to do multiplication on:";
		cin >> randomSize;
		if (randomSize == 10 || randomSize == 20 || randomSize == 30)
		{
			cout << endl;
			break;
		}
	} */	

	srand(time(NULL));

	// Creat the struct
	struct matrixMultiplicationProps *matProps = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
	
	// Declare Matrix A and Matrix B that will be multiplied together later on.
	matProps->numbers = new int [randomSize*randomSize];
	matProps->randomSize = randomSize;

	// Populate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		//matProps->numbers[i] = rand() % (randomSize*randomSize); 
		matProps->numbers[i] = i+1;
	}

	//display((void *) matProps);

	// Normal function call to do transformation
	//transformDiagNorm((void *) matProps);

	//display((void *) matProps);

	// Repopulate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		//matProps->numbers[i] = rand() % (randomSize*randomSize); 
		matProps->numbers[i] = i+1;
	}
	//display((void *) matProps);

		//pthread_t thread;
		//pthread_create(&thread, NULL, transformDiagPthread, (void *)matProps);
		//pthread_join(thread, NULL);

	// Threading
	display((void *) matProps);
	pthread_t threads[randomSize];
	for (int i = 0; i < randomSize-1; i++) // can be either randomSize or randomSize - 1. Dont 		need last thread as it only tries to compute the last value which does not move anyways.
	{

		// Create the struct for the thread
		matrixMultiplicationProps *mat = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
		mat->numbers = matProps->numbers;
		mat->randomSize = matProps->randomSize; 
		mat->rowEvaluate = i+1;	
		pthread_create(&threads[i], NULL, transformDiagPthread, (void *)mat);
	}
	
	// Join threads
	for (int i = 0; i < randomSize-1; i++)
	{	
		pthread_join(threads[i], NULL);

	}

	display((void *) matProps);

 return 0;
}
