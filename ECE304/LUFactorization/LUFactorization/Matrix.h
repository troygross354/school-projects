#pragma once
#include <iostream>
using namespace std;
class matrix {
public:
	matrix();
	matrix(int m, int  n);
	int  getRow();
	int  getCol();
	double& operator()(int, int);
	friend ostream& operator << (ostream& os, matrix& m);

	void luFact();
	void solveX(int, double*);
private:
	void  init(int, int);
	int  nrows, ncols;
	double  *data;
	matrix* L;
	matrix* U;
};
