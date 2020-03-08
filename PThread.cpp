#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>
#include <iomanip>
#include <time.h> 
#include <math.h>

using namespace std;

struct matrixDiagTransformProps{

	int randomSize;
	int* numbers = NULL;
	int rowStartEvaluate = 0;
	int numRowsEvaluate = 0;
	
};

struct matrixBlockTransformProps{

	int randomSize;
	int* numbers = NULL;
	//int id = 0;
	int blockSize = 0;
	int blockDim = 0;
	//int count = 0;

	int startPlaceHolder1 = 0;
	int endPlaceHolder1 = 0;
	int startPlaceHolder2 = 0;
	int endPlaceHolder2 = 0;
	int offset = 0;
	int* transpose1 = NULL;
	int* transpose2 = NULL;
	
};

// Display findl result matrix
void display(void* args)
{

	// Creat the struct
	struct matrixDiagTransformProps *matPropsDisp = (struct matrixDiagTransformProps*)args;

	int count = 0;
	int mat = 0;
	// Displaying matrix.	
    	cout << "Matrix:" << endl << endl;
	cout << setw(8);
   	for (int i = 0; i < matPropsDisp->randomSize*matPropsDisp->randomSize ; i++)
	{
		cout << matPropsDisp->numbers[i] << setw(8);
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

	struct matrixDiagTransformProps *matTemp = (struct matrixDiagTransformProps*)args;

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
	matrixDiagTransformProps *matTemp = (struct matrixDiagTransformProps*)args;

	for (int k = 0; k < matTemp->numRowsEvaluate; k++)
	{
		int temp;
		int multi = 1;
		int rowOffset = matTemp->rowStartEvaluate + 1;
		int i = ((matTemp->rowStartEvaluate)*(matTemp->randomSize))+rowOffset;
		
		for(i; i < (rowOffset)*(matTemp->randomSize); i++)
		{
			// Do transposition.
			temp = matTemp->numbers[i];
			matTemp->numbers[i] = matTemp->numbers[i+multi*(matTemp->randomSize-1)];
			matTemp->numbers[i+multi*(matTemp->randomSize-1)] = temp;
			multi++;
		}
		matTemp->rowStartEvaluate++;
		
	}
	return (NULL);	
}

void* transformBlock(void* args)
{

	struct matrixBlockTransformProps *matTemp = (struct matrixBlockTransformProps*)args;

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

void* transformBlocks(void* args)
{

	struct matrixBlockTransformProps *matTemp = (struct matrixBlockTransformProps*)args;

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

		temp = matTemp->transpose2[i];
		matTemp->transpose2[i] = matTemp->transpose2[i+multi*(matTemp->blockDim-1)];
		matTemp->transpose2[i+multi*(matTemp->blockDim-1)] = temp;
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

void displayBlock(void* args)
{

	// Creat the struct
	struct matrixBlockTransformProps *matPropsDisp = (struct matrixBlockTransformProps*)args;

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

void displayBlocks(void* args)
{

	// Creat the struct
	struct matrixBlockTransformProps *matPropsDisp = (struct matrixBlockTransformProps*)args;

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


	count = 0;
	mat = 0;
	// Displaying matA	
    	cout << "2-D Matrix A:" << endl << endl;
	cout << setw(5);
   	for (int i = 0; i < matPropsDisp->blockDim*matPropsDisp->blockDim ; i++)
	{
		cout << matPropsDisp->transpose2[i] << setw(5);
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
	matrixBlockTransformProps *matTemp = (struct matrixBlockTransformProps*)args;
	int rowCount = 1;
	int offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	int placeHolder = 0;

	
	// For loop to grab values that need to be transposed.
	for (int i = matTemp->startPlaceHolder1; i <= matTemp->endPlaceHolder1; i++)
	{	
		matTemp->transpose1[placeHolder] = matTemp->numbers[i];
		//cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}
	//displayBlock((void *) matTemp);

	// Transpose the block
	transformBlocks((void *) matTemp);
	
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
	//displayBlock((void *) matTemp);
	//cin.get();

}

void* BlockTransformSwap(void* args)
{

	// Make a local pointer to the struct
	matrixBlockTransformProps *matTemp = (struct matrixBlockTransformProps*)args;
	int rowCount = 1;
	int offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	int placeHolder = 0;

	
	// For loop to grab values that need to be transposed in first block
	for (int i = matTemp->startPlaceHolder1; i <= matTemp->endPlaceHolder1; i++)
	{	
		matTemp->transpose1[placeHolder] = matTemp->numbers[i];
		//cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}
	
	rowCount = 1;
	offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	placeHolder = 0;

	// For loop to grab values that need to be transposed in second block
	for (int i = matTemp->startPlaceHolder2; i <= matTemp->endPlaceHolder2; i++)
	{	
		matTemp->transpose2[placeHolder] = matTemp->numbers[i];
		//cout << matTemp->numbers[i] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}

	//displayBlocks((void *) matTemp);

	// Transpose the blocks
	transformBlocks((void *) matTemp);

	//displayBlocks((void *) matTemp);
	
	// Swap The Blocks
	int temp[matTemp->blockDim*matTemp->blockDim] = {}; 
	rowCount = 1;
	offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	placeHolder = 0;
	for (int i = matTemp->startPlaceHolder1; i <= matTemp->endPlaceHolder1; i++)
	{
		temp[placeHolder] = matTemp->transpose1[placeHolder];
		matTemp->numbers[i] = matTemp->transpose2[placeHolder];
		//cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}

	rowCount = 1;
	offset = matTemp->randomSize - matTemp->blockDim; //- matTemp->id*matTemp->blockDim;
	placeHolder = 0;
	for (int i = matTemp->startPlaceHolder2; i <= matTemp->endPlaceHolder2; i++)
	{
		matTemp->numbers[i] = temp[placeHolder];
		//cout << matTemp->transpose1[placeHolder] << "      " << placeHolder << endl;
		rowCount++;
		
		if(rowCount == matTemp->blockDim+1)
		{
			i += offset;
			rowCount = 1;
			
		}
		placeHolder++;
	}





}

int main ( int argc, char* argv[] )
{
	// Define clock
	clock_t start_t, end_t;
	int randomSize = 16;

	// User input
	while (randomSize != 128 | randomSize != 1024 | randomSize != 2048 | randomSize != 4096)
	{
		cout << "Please enter either, 128, 1024, 2048 or 4096 for the size of the matrix you want to do transform: ";
		cin >> randomSize;
		if (randomSize == 128 || randomSize == 1024 || randomSize == 2048 || randomSize == 4096)
		{
			cout << endl;
			break;
		}
	}
	
	srand(time(NULL));

	////////////////////// Normal Transformation (No threading) ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	// Create the struct
	/*struct matrixDiagTransformProps *matPropsDiag = (struct matrixDiagTransformProps*) malloc(sizeof(struct matrixDiagTransformProps));
	
	// Declare Matrix A and Matrix B that will be multiplied together later on.
	matPropsDiag->numbers = new int [randomSize*randomSize];
	matPropsDiag->randomSize = randomSize;

	// Populate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		matPropsDiag->numbers[i] = rand() % randomSize; 
		//matProps->numbers[i] = i+1;// Used for testing purposes
	}


	//display((void *) matProps);
	//cin.get();

	// Normal function call to do transformation
	//transformDiagNorm((void *) matProps);

	//display((void *) matProps);*/
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////// PThread Diagonal Transformation /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	
	// Create the struct
	struct matrixDiagTransformProps *matProps = (struct matrixDiagTransformProps*) malloc(sizeof(struct matrixDiagTransformProps));
	
	// Declare matrix for transformation
	matProps->numbers = new int [randomSize*randomSize];
	matProps->randomSize = randomSize;

	// Populate
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		matProps->numbers[i] = rand() % randomSize*randomSize; 
		//matProps->numbers[i] = i+1; // Used for testing purposes
	}


	//display((void *) matProps); // Display matrix before transformation. Used for testing purposes
	const int numThread = 4;
	pthread_t threads[numThread];
	int lastStart = 0;
	
	start_t = clock();
	for (int i = 0; i < numThread; i++) // can be either randomSize or randomSize - 1. Dont 		need last thread as it only tries to compute the last value which does not move anyways.
	{

		// Create the struct for the thread
		matrixDiagTransformProps *mat = (struct matrixDiagTransformProps*) malloc(sizeof(struct matrixDiagTransformProps));
		mat->numbers = matProps->numbers;
		mat->randomSize = matProps->randomSize; 
		mat->rowStartEvaluate = lastStart;
		lastStart = mat->rowStartEvaluate + randomSize/numThread;
		mat->numRowsEvaluate = randomSize/numThread;
		pthread_create(&threads[i], NULL, transformDiagPthread, (void *)mat);
	}
	
	// Join threads
	for (int i = 0; i < numThread; i++)
	{	
		pthread_join(threads[i], NULL);
	}
	
	end_t = clock();
	//display((void *) matProps); // Display matrix after transformation. Used for testing purposes
	cout << "PThread Using Diagonal Method For Transformation of A Matrix Results:" << endl << endl;
	cout << "Number of Threads: " << numThread << endl;
	cout << "Rows Transposed Per Thread: " << randomSize/numThread << endl;
	cout << "Time taken to execute: " << (double)(end_t - start_t)/CLOCKS_PER_SEC << "s" << endl << endl << endl;
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	////////////////////// PThread Block Method Transformation ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////


	// Create the struct
	struct matrixBlockTransformProps *matProps2 = (struct matrixBlockTransformProps*) malloc(sizeof(struct matrixBlockTransformProps));

	matProps2->numbers = new int [randomSize*randomSize];
	
	matProps2->randomSize = randomSize;

	// Repopulate Array
	for (int i = 0; i < (randomSize*randomSize); i++)
	{  
		matProps2->numbers[i] = rand() % randomSize; 
		//matProps2->numbers[i] = i+1; // Used for testing purposes
	}

	// This was used originally to change the number of threads based on the size of the blocks wanted
	/*int blockSizeDim = 4;
	int blockSize = blockSizeDim*blockSizeDim;
	int numBlocks = (randomSize*randomSize)/blockSize;
	int numThreads = ((numBlocks - (randomSize/blockSizeDim))/2) + (randomSize/blockSizeDim);
	matProps->transpose1 = new int [blockSize];
	matProps->transpose2 = new int [blockSize];*/


	// Calculate the how many blocks and the dimensions of the blocks needed to execute the transposition 		based on the number of threads defined.
	const int numberThreads = 10;
	int numBlocks = numberThreads;
	
	//Block size will increase to suit number of threads
	while ((randomSize*randomSize)%numBlocks != 0)
	{
		numBlocks++;
	}
	int blockSize = (randomSize*randomSize)/numBlocks;
	int blockSizeDim = sqrt(blockSize);	

	matProps2->transpose1 = new int [blockSize];
	matProps2->transpose2 = new int [blockSize];

	pthread_t threadsBlock[numberThreads];
	int count = 0;
	int i = 0;
	
	// display((void *) matProps2); // Display matrix before transformation. Used for testing
	// For loop to deal with diagonal blocks separately as they only need transposing not switching.
	start_t = clock();
	for (i; i < randomSize/blockSizeDim; i++)
	{
		// Create the struct for the thread
		matrixBlockTransformProps *mat = (struct matrixBlockTransformProps*) malloc(sizeof(struct matrixBlockTransformProps));
		mat->numbers = matProps2->numbers;
		mat->transpose1 = matProps2->transpose1;
		mat->transpose2 = matProps2->transpose2;
		mat->randomSize = matProps2->randomSize; 
		mat->blockSize = blockSize;
		mat->blockDim = blockSizeDim;
		//mat->count = count;
		//mat->id = i+1;
		//cout << i << endl;
		mat->startPlaceHolder1 = (count*randomSize*blockSizeDim) + (i*blockSizeDim);
		mat->endPlaceHolder1 = mat->startPlaceHolder1 + blockSizeDim + ((blockSizeDim-1)*randomSize) - 1;
		//cout << mat->startPlaceHolder1 << "		" << mat->endPlaceHolder1 << endl;
		pthread_create(&threadsBlock[i], NULL, DiagBlockTransform, (void *)mat);
		//cin.get();
		count++;	
	}
	
	// For loop to deal with rest of blocks transposition and switching
	count = 1;
	int row = 1;
	int blockNumStart = 2;
	int blockNum = 2;
	int blockNumOriginal = blockNum;
	int offset = 0;
	int blockOffset = 1;
	int secondMatOffset = randomSize - blockNum*blockSizeDim;
	
	for (i; i < numberThreads; i++)
	{
		//cout << i << endl;
		// Create the struct for the thread
		matrixBlockTransformProps *mat = (struct matrixBlockTransformProps*) malloc(sizeof(struct matrixBlockTransformProps));
		mat->numbers = matProps2->numbers;
		mat->transpose1 = matProps2->transpose1;
		mat->transpose2 = matProps2->transpose2;
		mat->randomSize = matProps2->randomSize; 
		mat->blockSize = blockSize;
		mat->blockDim = blockSizeDim;
		//mat->count = count;

		mat->startPlaceHolder1 = blockSizeDim*(blockNum-1) + offset;
		mat->endPlaceHolder1 = mat->startPlaceHolder1 + blockSizeDim + ((blockSizeDim-1)*randomSize) - 1;
		mat->startPlaceHolder2 = mat->endPlaceHolder1 + secondMatOffset+1;
		mat->endPlaceHolder2 = mat->startPlaceHolder2 + blockSizeDim + ((blockSizeDim-1)*randomSize) - 1;
		//cout << mat->startPlaceHolder1 << "		" << mat->endPlaceHolder1 << endl;
		//cout << mat->startPlaceHolder2 << "		" << mat->endPlaceHolder2 << endl;
		blockNum++;
		secondMatOffset += randomSize*blockSizeDim - blockSizeDim;
		//cout << endl << "MatOffset		" << secondMatOffset << endl;
		pthread_create(&threadsBlock[i], NULL, BlockTransformSwap, (void *)mat);
		//display((void *) matProps);
		//cin.get();
		// If you reach the end of a row of blocks, go to the next row
		if(mat->endPlaceHolder1 == row*randomSize*blockSizeDim - 1 && i < numberThreads-1)
		{
			offset += randomSize*blockSizeDim;
			blockNumStart++;
			row++;
			blockNum = blockNumStart;
			secondMatOffset = randomSize - blockNumOriginal*blockSizeDim;
			//cout << endl << "got here		" << offset << endl;
		}
		

	}	

	// Join threads
	for (int i = 0; i < numberThreads; i++)
	{	
		pthread_join(threadsBlock[i], NULL);

	}
	end_t = clock();

	cout << "PThread Using Blocks Method For Transformation of A Matrix Results:" << endl << endl; 
	cout << "Number of Threads: " << numberThreads << endl;
	cout << "Number of Blocks: " << numBlocks << endl;
	cout << "Block Size (Number of Elements in Block): " << blockSize << endl;
	cout << "Block Dimension: " << blockSizeDim << endl;
	cout << "Time taken to execute: " << (double)(end_t - start_t)/CLOCKS_PER_SEC << "s" << endl;

	//display((void *) matProps2); // Display final matrix after transformation. Used for testing

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
 return 0;
}
