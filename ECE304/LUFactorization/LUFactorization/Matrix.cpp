#include "Matrix.h"
/*****************************| Supplied Code |*********************************/
matrix::matrix() {
	init(1, 1);
}

matrix::matrix(int m, int n) {
	init(m, n);
}

void  matrix::init(int m, int n) {
	nrows = m;
	ncols = n;
	data = new double[m*n];
	for (int i = 0; i<m*n; i++)
		data[i] = 0;
}

int matrix::getRow() { return nrows; }
int matrix::getCol() { return ncols; }

double& matrix::operator() (int r, int c) {
	if (r < 0 || r > nrows) {
		cout << "Illegal Row Index:";
		return data[0];
	}
	else if (c < 0 || c > ncols) {
		cout << "Illegal Column Index:";
		return data[0];
	}
	else return data[r*ncols + c];
}

ostream& operator<<(ostream &os, matrix &m) {
	int mval = m.getRow();
	int nval = m.getCol();
	for (int i = 0; i<mval; i++) {
		for (int j = 0; j < nval; j++)
			os << m(i, j);
		os << endl;
	}
	return os;
}

/*****************************| Added Code |*********************************/
void matrix::luFact()
{
	//Make sure matrix is square
	if (this->nrows != this->ncols) return;

	//Instantiate general dimension variable
	int n = nrows;

	//Instantiate L and U matricies
	L = new matrix(n, n);
	U = new matrix(n, n);

	//Evaluate LU
	for (int pivot = 0; pivot < n; pivot++)
	{
		//L Update
		for (int row = pivot; row < n; row++)
		{
			if (row == pivot)
				L->data[] = 1;
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

//TODO: Should return a vector x
void solveX(int n, double *b)
{
	this->luFact();
	double *z = new double[n];
	for (int i = 0; i < n; i++)
		z[i] = 0;
	double sum = 0;

	//TODO: Instantiate x vector
	matrix x

	//Solve Lz = B for z
	for (int i = 0; i < n; i++)
	{
		//Sum(Lij, zj) from i 
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
}