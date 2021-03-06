#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "Multiplication.h"
using namespace std;

void Multiplication::KaratsubaRecursive(int* x, int* y, long int size, int* res)
{	// x*y = (a*c)*10^n + (b*d) + ((a+b)*(c+d)-a*c-b*d)* 10^(n/2) 

	if (size == 1)
	{	// res size = 2
		int mul = x[0] * y[0];
		res[1] = mul % 10;
		res[0] = mul / 10;
		return;
	}

	int *a, *b, *c, *d, *ac, *bd;
	long int firstHalf, secondHalf, acSize, bdSize;

	firstHalf = size / 2;
	secondHalf = size - (size / 2);
	acSize = (size / 2) * 2;
	bdSize = (size - (size / 2)) * 2;

	a = x;
	b = x + firstHalf;
	c = y;
	d = y + firstHalf;
	ac = res;
	bd = res + acSize;

	KaratsubaRecursive(a, c, firstHalf, ac);	// acSize = (size / 2) * 2
	KaratsubaRecursive(b, d, secondHalf, bd);	// bdSize = (size - (size / 2)) * 2

    long int aPlusbSize = 0, cPlusdSize = 0, prodOfSumSize = 0;
	// (a+b)
	int* aPlusb = adder(a, b, firstHalf, secondHalf, aPlusbSize);	// aPlusbSize = (size - (size / 2)) + 1
	// (c+d)
	int* cPlusd = adder(c, d, firstHalf, secondHalf, cPlusdSize);	// cPlusdSize = (size - (size / 2)) + 1

	// (a+b)*(c+d)
    int* prodOfSum;
	if (aPlusb[0] == 0 && cPlusd[0] == 0) {     // removes leading zero
		prodOfSumSize = (aPlusbSize - 1) * 2;	// aPlusbSize == cPlusdSize
		prodOfSum = new int[prodOfSumSize]();
		KaratsubaRecursive(aPlusb + 1, cPlusd + 1, aPlusbSize - 1, prodOfSum);	// prodOfSumSize = (size - (size / 2)) * 2
	}
	else {
		prodOfSumSize = aPlusbSize * 2;
		prodOfSum = new int[prodOfSumSize]();
		KaratsubaRecursive(aPlusb, cPlusd, aPlusbSize, prodOfSum);	// prodOfSumSize = (size - (size / 2)) * 2 + 2
	}
    
	delete[] aPlusb;
	delete[] cPlusd;

	//(a+b)*(c+d) - (a*c)
    subtractor(prodOfSum, ac, prodOfSumSize, acSize);
	//(a+b)*(c+d) - (a*c) - (b*d)
    subtractor(prodOfSum, bd, prodOfSumSize, bdSize);
	// (ac)*10^size + middle * 10^(size/2) + (bd)
    sumMiddlePart(res, prodOfSum, size, prodOfSumSize);

    delete [] prodOfSum;
	return;
}


void Multiplication::KaratsubaRecursiveHelper(int* x, int* y, long int size)
{
	int* res = new int[size * 2]();

	auto start = chrono::high_resolution_clock::now();
	// unsync the I/O of C and C++.
	ios_base::sync_with_stdio(false);
	KaratsubaRecursive(x, y, size, res);
	auto end = chrono::high_resolution_clock::now();
	// Calculating total time taken by the program.
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	time_taken *= 1e-9;
	ofstream myfile("Measure.txt", ios::app); // The name of the file
	myfile << "Time taken by function KaratsubaRecursive is : " << fixed << time_taken << setprecision(9);
	myfile << " sec" << endl;
	myfile.close();

	cout << "\nKaratsuba(recursive) : x * y = ";
	printIntArr(res, 2 * size);
	delete[] res;
}



