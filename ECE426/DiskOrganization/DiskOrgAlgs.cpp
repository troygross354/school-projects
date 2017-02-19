#include <iostream>
#include <random>
using namespace std;

void quickSort(int* arr, int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2];
 
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
 
      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}
int binarySearch(int* arr, int value, int left, int right) {
	while (left <= right) {
		int middle = (left + right) / 2;
		if (arr[middle] == value)
			return middle;
		else if (arr[middle] > value)
			right = middle - 1;
		else
			left = middle + 1;
	}
	return -1;
}

//First come first serve
void fcfs(int n, int a[])
{
	double mean = 0, variance = 0;

	//Calc mean
	for(int i = 1; i < n; i++)
	{
		mean += abs(a[i] - a[i - 1]);
	}

	mean = mean / (n - 1);

	//Calc Variance
	for(int i = 1; i < n; i++)
	{
		variance += pow(abs(a[i] - a[i - 1]) - mean, 2);
	}

	variance = variance / (n - 1);

	cout << "FCFS Mean: " << mean << endl;
	cout << "FCFS Variance: " << variance << endl;

}

//Shortest seek time first
void sstf(int n, int a[])
{
	//Save starting point before sorting
	int firstVal = a[0], lPtr, hPtr, mPtr, index = 0;
	int* moveList = new int[n - 1];
	int* ssArr = new int[n];
	int highMove, lowMove;
	double mean = 0, variance = 0; 

	//Copy array into dynamically allocated array
	for (int i = 0; i < n; i++)
	{
		ssArr[i] = a[i];
	}

	//Sort array in ascending order
	quickSort(ssArr, 0, n - 1);

	//Find index of the starting value in the sorted array
	mPtr = binarySearch(ssArr, firstVal, 0, n - 1);

	//Set pointers equal to starting index of fisrtVal
	hPtr = mPtr + 1;
	lPtr = mPtr - 1;

	highMove = abs(ssArr[mPtr] - ssArr[hPtr]);
	lowMove = abs(ssArr[mPtr] - ssArr[lPtr]);

	if (highMove < lowMove)
	{
		moveList[index++] = highMove;
		++hPtr;
	}
	else
	{
		moveList[index++] = lowMove;
		--lPtr;
	}

	//Find all of the move differences
	while (lPtr > 0 && hPtr < n - 1)
	{
		if (hPtr < n - 1)
		{
			//Calculate the high move
			highMove = abs(ssArr[hPtr] - ssArr[hPtr - 1]);
		}
		if (lPtr > 0)
		{
			//Calculate the low move
			lowMove = abs(ssArr[lPtr] - ssArr[lPtr + 1]);
		}

		if (highMove < lowMove)
		{
			//Take the high move
			moveList[index++] = highMove;
			++hPtr;
		}
		else
		{
			//Take the low move
			moveList[index++] = lowMove;
			--lPtr;
		}
	}
	
	if (lPtr == 0)
	{
		//Low pointer bottomed out continue from the high
		//Last low move
		lowMove = abs(ssArr[lPtr] - ssArr[lPtr + 1]);
		moveList[index++] = lowMove;
	
		highMove = abs(ssArr[lPtr] - ssArr[hPtr]); //Transition from low to high
		moveList[index++] = highMove;
		while (hPtr < n)
		{
			highMove = abs(ssArr[hPtr] - ssArr[hPtr - 1]);
			moveList[index++] = highMove;
			++hPtr;
		}

	}
	else
	{
		//High pointer topped out
		highMove = abs(ssArr[hPtr] - ssArr[hPtr - 1]);//last high
		moveList[index++] = highMove;
		lowMove = abs(ssArr[hPtr] - ssArr[lPtr]);//Big jump from high to low pointer
		moveList[index++] = lowMove;

		while (lPtr >= 0)
		{
			lowMove = abs(ssArr[lPtr] - ssArr[lPtr + 1]);
			moveList[index++] = lowMove;
			--lPtr;
		}
	}

	//Calc mean
	for(int i = 0; i < n - 1; i++)
	{
		mean += moveList[i];
	}

	mean = mean / (n - 1);

	//Calc Variance
	for(int i = 0; i < n - 1; i++)
	{
		variance += pow(moveList[i] - mean, 2);
	}

	variance = variance / (n - 1);

	cout << "SSTF Mean: " << mean << endl;
	cout << "SSTF Variance: " << variance << endl;
}

void scan(int n, int a[], int dir)
{
	//Save starting point before sorting
	int firstVal = a[0], firstValIndex, mlIndex = 0;
	int* scanArr = new int[n];
	int* moveList = new int[n - 1];
	double mean = 0, variance = 0;

	//Copy array into dynamically allocated array
	for (int i = 0; i < n; i++) scanArr[i] = a[i];

	//Sort array in ascending order
	quickSort(scanArr, 0, n - 1);

	//Find index of the starting value in the sorted array
	firstValIndex = binarySearch(scanArr, firstVal, 0, n - 1);

	//Calc mean
	if (dir == 0)//Increasing direction
	{
		for (int i = firstValIndex; i < n; i++)
		{
			moveList[mlIndex++] = abs(scanArr[i] - scanArr[i - 1]);
		}
		if (firstValIndex != 0)//Check that the first value index is not zero
		{
			moveList[mlIndex++] = abs(scanArr[n - 1] - scanArr[firstValIndex - 1]);//Big jump from high to first value - 1
			for (int i = firstValIndex - 1; i > 0; i--)
				moveList[mlIndex++] = abs(scanArr[i] - scanArr[i - 1]);
		}
		
	}
	else//Deacreasing direction
	{
		for (int i = firstValIndex; i > 0; i--)
		{
			moveList[mlIndex++] = abs(scanArr[i] - scanArr[i - 1]);
		}
		if (firstValIndex !=  n - 1)//Check that the first value index is not zero
		{
			moveList[mlIndex++] = abs(scanArr[0] - scanArr[firstValIndex + 1]); //Big jump from zero
			for (int i = firstValIndex + 1; i < n - 1; i++)
			{
				moveList[mlIndex++] = abs(scanArr[i] - scanArr[i - 1]);
			}
		}
	}
	
	for (int i = 0; i < n - 1; i++)
	{
		mean += moveList[i];
	}

	//Calc mean
	mean = mean / (n - 1);

	//Calc Variance
	for (int i = 0; i < n - 1; i++)
	{
		variance += pow(moveList[i] - mean, 2);
	}

	variance = variance / (n - 1);

	cout << "SCAN Mean: " << mean << endl;
	cout << "SCAN Variance: " << variance << endl;
}

int main()
{
	//Simple array from powerpoint to test.
	int firstArr[] = {63, 33, 72, 47, 8, 99, 74, 52, 75};

	//Generate random numbers from 0 to 99 and store randomly in an array
	int arr[100000];
	int n;

	cout << "Results for {63, 33, 72, 47, 8, 99, 74, 52, 75}: " << endl << endl;
	fcfs(9, firstArr);
	sstf(9, firstArr);
	scan(9, firstArr, 0);

	cout << endl;

	//Fill the array at random indeces with sequential values
	for(int trial = 0; trial < 4; trial++)
	{
		n = (int)pow(10, trial + 2);
		cout << "With " << n << " random values between (0-" << n - 1 << ")..." << endl << endl;
		for(int i = 0; i < n; i++)
		{
			arr[i] = rand() % n;
		}
		fcfs(n, arr);
		sstf(n, arr);
		scan(n, arr, 0);

		cout << endl;
	}
	return 0;
}