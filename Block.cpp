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
	int id = 0;
	int blockSize = 0;
	int blockDim = 0;
	int count = 0;

	int startPlaceHolder1 = 0;
	int endPlaceHolder1 = 0;
	int startPlaceHolder2 = 0;
	int offset = 0;
	int* transpose1 = NULL;
	int* transpose2 = NULL;
	
};

void* transformBlock(void* args)
{

	struct matrixMultiplicationProps *matTemp = (struct matrixMultiplicationProps*)args;

	int temp;
	int row = 2;
	int multi = 1;
	int i = 1;

	for(i; i < matTemp->blockDim*matTemp->blockDim; i++)
	{
		//cout << matTemp->transpose1[i] << endl;
		// Do transposition.
		temp = matTemp->transpose1[i];
		matTemp->transpose1[i] = matTemp->transpose1[i+multi*(matTemp->blockDim-1)];
		matTemp->transpose1[i+multi*(matTemp->blockDim-1)] = temp;
		multi++;
		
		// If the end of the row is reached. 7, 15, 23
		if (i == (row - 1)*(matTemp->blockDim)-1)
		{	
			i += row; // Start at a new i which is offset to start at next row.
			row++;
			multi = 1; // Reset multiplier to get correct values to transpose.
		}

	}
	return (NULL);
}

// Display entire matrix
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

void displayBlock(void* args)
{

	// Creat the struct
	struct matrixMultiplicationProps *matPropsDisp = (struct matrixMultiplicationProps*)args;

	int count = 0;
	int mat = 0;
	// Displaying matA	
    	cout << "2-D Matrix A:" << endl << endl;
	cout << setw(5);
   	for (int i = 0; i < matPropsDisp->blockDim*matPropsDisp->blockDim ; i++)
	{
		cout << matPropsDisp->transpose1[i] << setw(5);
		count++;
		mat++;
		if (mat == matPropsDisp->blockDim*matPropsDisp->blockDim)
		{
			cout << endl;
			mat = 0;
		}
		if (count == matPropsDisp->blockDim)
		{
			cout << endl;
			count = 0;
		}
	}
}

void* DiagBlockTransform(void* args)
{
	// Make a local pointer to the struct
	matrixMultiplicationProps *matTemp = (struct matrixMultiplicationProps*)args;
	int rowCount = 1;
	int offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	int placeHolder = 0;

	
	// For loop to grab values that need to be transposed.
	for (int i = matTemp->startPlaceHolder1; i <= matTemp->endPlaceHolder1; i++)
	{	
		matTemp->transpose1[placeHolder] = matTemp->numbers[i];
		cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}
	displayBlock((void *) matTemp);

	// Transpose the block
	transformBlock((void *) matTemp);
	
	// Replace values in final matrix with new blocks.
	placeHolder = 0;
	for (int i = matTemp->startPlaceHolder1; i <= matTemp->endPlaceHolder1; i++)
	{
		matTemp->numbers[i] = matTemp->transpose1[placeHolder];
		//cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}
	displayBlock((void *) matTemp);
	cin.get();

}

int main ( int argc, char* argv[] )
{
	// Define clock
	clock_t t;
	int randomSize = 16;

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

	// Repopulate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		matProps->numbers[i] = i+1;
	}

	// Threading
	//display((void *) matProps);


	int blockSizeDim = 4;
	int blockSize = blockSizeDim*blockSizeDim;
	int numBlocks = (randomSize*randomSize)/blockSize;
	int numThreads = ((numBlocks - (randomSize/blockSizeDim))/2) + (randomSize/blockSizeDim);
	matProps->transpose1 = new int [blockSize];
	//cout << numThreads << endl;
	//cin.get();
	pthread_t threads[numThreads];
	int count = 0;
	int i = 0;	


	// for loop to deal with diagonal blocks separately as they only need transposing not switching.
	for (i; i < randomSize/blockSizeDim; i++)
	{
		// Create the struct for the thread
		matrixMultiplicationProps *mat = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
		mat->numbers = matProps->numbers;
		mat->transpose1 = matProps->transpose1;
		mat->randomSize = matProps->randomSize; 
		mat->blockSize = blockSize;
		mat->blockDim = blockSizeDim;
		mat->count = count;
		mat->id = i+1;
		//cout << i << endl;
		mat->startPlaceHolder1 = (count*randomSize*blockSizeDim) + (i*blockSizeDim);
		mat->endPlaceHolder1 = mat->startPlaceHolder1 + blockSizeDim + ((blockSizeDim-1)*randomSize) - 1;
		cout << mat->startPlaceHolder1 << "		" << mat->endPlaceHolder1 << endl;
		pthread_create(&threads[i], NULL, DiagBlockTransform, (void *)mat);
		cin.get();
		count++;	
	}
	
	// for loop to deal with rest of blocks transposition and switching
	//for (i; i < numThreads - 1; i++)
	//{

	//}	
	
	// Join threads
	//for (int i = 0; i < randomSize/blockSizeDim; i++)
	//{	
	//	pthread_join(threads[i], NULL);

	//}

	display((void *) matProps);

 return 0;
}
