#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>
#include <iomanip>
#include <time.h> 

using namespace std;

struct matrixMultiplicationProps{
	int id = 1;
	int randomSize;
	int* numbers = NULL;
	
};

void display(void* args)
{

	// Creat the struct
	struct matrixMultiplicationProps *matPropsDisp = (struct matrixMultiplicationProps*)args;

	int count = 0;
	int mat = 0;
	// Displaying matA	
    	cout << "2-D Matrix A:" << endl << endl;
	cout << setw(3);
   	for (int i = 0; i < matPropsDisp->randomSize*matPropsDisp->randomSize ; i++)
	{
		cout << matPropsDisp->numbers[i] << setw(3);
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

// Normal 2d matrix multiplication.
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

	display((void *) matProps);

	// Normal function call to do transformation
	transformDiagNorm((void *) matProps);

	display((void *) matProps);

	// Repopulate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		//matProps->numbers[i] = rand() % (randomSize*randomSize); 
		matProps->numbers[i] = i+1;
	}
	display((void *) matProps);


	
	

	/*
	// Setting variables of struct
	matProps->blockSizeForThread = blockSize;
	pthread_t threads[randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	t = clock(); // Initialize timer

	// Threading
	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		pthread_create(&threads[i], NULL, transformDiagPthread, (void *)matProps);
	}
	
	// Join threads
	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		pthread_join(threads[i], NULL);

	}*/

 return 0;
}
