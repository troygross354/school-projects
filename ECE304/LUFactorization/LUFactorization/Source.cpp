#include <iostream>
#include <iomanip>
#include "stdlib.h"

using namespace std;

//Performs LU Factorization of input "mat into L and U
void luFact(int n, double **mat, double **L, double **U)
{

	//Copy input mat to U mat
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			U[i][j] = mat[i][j];
		}
	}
	
	//Evaluate LU
	for (int pivot = 0; pivot < n; pivot++)
	{
		//L Update
		for (int row = pivot; row < n; row++)
		{
			if (row == pivot)
				L[row][pivot] = 1;
			else
				L[row][pivot] = U[row][pivot] / U[pivot][pivot];
		}

		//U Update
		//Row iterator
		for (int row = pivot + 1; row < n; row++)
		{
			//Column iteration for Ri -(Ri[0]/pivot[0])Rpivot
			for (int col = pivot; col < n; col++)
			{
				U[row][col] -= (U[pivot][row] / U[pivot][pivot]) * U[pivot][col];
			}
		}
	}
}

//Solves equation Ax=b, stores result vector in x
void solveX(int n, double **mat, double *x, double *b, double **L, double **U)
{
	//Find the LU factorization first
	luFact(n, mat, L, U);

	//Instantiate a temporary z array
	double *z = new double[n];
	for (int i = 0; i < n; i++)
		z[i] = 0;
	double sum = 0;

	//Solve Lz = B for z
	for (int i = 0; i < n; i++)
	{
		//Sum(Lij, zj)
		for (int j = 0; j < i; j++)
		{
			sum += L[i][j] * z[j];
		}
		z[i] = (b[i] - sum) / L[i][i];
		sum = 0;
	}

	//Solve Ux = z for x
	for (int i = n - 1; i >= 0; i--)
	{
		//Sum(Uij, j) from i to n-1
		for (int j = i; j < n; j++)
		{
			sum += U[i][j] * x[j];
		}
		x[i] = (z[i] - sum) / U[i][i];
		sum = 0;
	}
	//Deallocate z
	delete[] z;
}

//Calculates the inverse of an nxn matrix
void inverse(int n, double **mat, double **invOut)
{
	double **bigMat = new double*[n];
	//Allocate memory for big matrix (n x 2n)
	for (int i = 0; i < n; i++)
		bigMat[i] = new double[2 * n];

	//Copy input matrix into left side of big matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			bigMat[i][j] = mat[i][j];
		}
	}
	//Build identity section
	for (int i = 0; i < n; i++)//Row iteration
	{
		for (int j = n; j < 2 * n; j++)//Column iteration
		{
			if (i + n == j)
				bigMat[i][j] = 1.0;
			else
				bigMat[i][j] = 0.0;
		}
	}

	//Gaussian elimination
	//Downward traversal
	for (int i = 0; i < n - 1; i++)//pivot
	{
		//Divide pivot row by diagonal term
		double diagTemp = bigMat[i][i];
		for (int j = i; j < 2 * n; j++)
		{
			bigMat[i][j] /= diagTemp;
		}
		//Multiply by lower rows lead term and add to them
		for (int row= i + 1; row < n; row++)
		{
			double rowCoeff = bigMat[row][i];
			for (int col = i; col < 2 * n; col++)
			{
				bigMat[row][col] -= bigMat[i][col] * rowCoeff;
			}
		}
	}

	//Upward traversal
	for (int i = n - 1; i >= 0; i--)//pivot
	{
		//Divide pivot row by diagonal term
		double diagTemp = bigMat[i][i];
		for (int j = i; j < 2 * n; j++)
		{
			bigMat[i][j] /= diagTemp;
		}
		//Multiply by upper rows lead term and add to them
		for (int row = i - 1; row >= 0; row--)
		{
			double rowCoeff = bigMat[row][i];
			for (int col = i; col < 2 * n; col++)
			{
				bigMat[row][col] -= bigMat[i][col] * rowCoeff;
			}
		}
	}
	
	//Copy right half of big matrix into inv
	for (int i = 0; i < n; i++)
	{
		for (int j = n; j < 2 * n; j++)
		{
			//Column offset from bigMat(n x 2n) to invOut(n x n) is: j % n
			invOut[i][j % n] = bigMat[i][j];
		}
		//Deallocate current row pointer
		delete[] bigMat[i];
	}
	//Deallocate pointer
	delete[] bigMat;
}

//Calculates the determinant of an already LU Factored matrix
double det(int n, double** matL, double** matU)
{
	double detVal = 1;

	//Multiply all of the diagonals to produce the determinant
	for (int i = 0; i < n; i++)
	{
		detVal *= matL[i][i] * matU[i][i];
	}
	return detVal;
}

void printMat(int n, double **matrix)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << setw(8) << matrix[i][j];
		}
		cout << endl << endl;
	}
}
void printVector(double *vect, int n)
{
	for (int i = 0; i < n; i++)
		cout << setw(8) << vect[i] << endl << endl;
}

int main()
{
	const int DIM = 4;
	int i, j;
	double **input, **L, **U, **inv, *b, *x;
	input = new double* [DIM];
	L = new double* [DIM];
	U = new double* [DIM];
	inv = new double* [DIM];
	b = new double[DIM];
	x = new double[DIM];

	//Test Values
	double imat[16] = {13, -4, -3, -6, -4, 17, -2, 0, -3, -2, 11, -2, -6, 0, -2, 12};
	double ib[4] = { 500, 0, 0, 0 };

	//Populate pointer arrays to double arrays
	for (i = 0; i < DIM; i++)
	{
		input[i] = new double[DIM];
		L[i] = new double[DIM];
		U[i] = new double[DIM];
		inv[i] = new double[DIM];
	}

	//Output format settings
	cout << fixed << setprecision(4);

	//Initialize input, L, and U matricies
	cout << "Input matrix: " << endl;
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			input[i][j] = imat[i * 4 + j];
			L[i][j] = U[i][j] = inv[i][j] = 0;
		}
	}
	printMat(DIM, input);
	cout << "Input b: " << endl;
	for (int i = 0; i < DIM; i++)
	{
		//cin >> b[i];
		b[i] = ib[i];
		x[i] = 0;
	}
	printVector(b, DIM);

	//Perform LU Factorization and solve for x
	solveX(DIM, input, x, b, L, U);

	//Calculate inverse of input matrix
	inverse(DIM, input, inv);

	//Print Input, L, U, x, and inverse and determinant
	cout << "Input matrix: " << endl;
	printMat(DIM, input);
	cout << "\nL matrix: " << endl;
	printMat(DIM, L);
	cout << "\nU matrix: " << endl;
	printMat(DIM, U);
	cout << "\nx: " << endl;
	printVector(x, DIM);
	cout << "Determinant of input matrix: " << det(DIM, L, U) << endl << endl;
	cout << "Inverse of input matrix:" << endl;
	printMat(DIM, inv);
	cout << endl;


	//Deallocate memory
	for (i = 0; i < DIM; i++)
	{
		delete[] input[i];
		delete[] L[i];
		delete[] U[i];
		delete[] inv[i];
	}
	delete[] input;
	delete[] L;
	delete[] U;
	delete[] inv;
	
	return 0;
}