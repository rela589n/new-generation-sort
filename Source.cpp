#include <iostream>
#include <ctime>
#include <algorithm>
#include "time_utilities.h"
#include "sort_utilities.h"

using namespace std;
using namespace rela589n;

int getAvarageStep(int* arr, int len);

void printArr(int* arr, int len) {
	for (int i = 0; i < len; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

bool arraysEqual(int* arr1, int* arr2, int len) {
	for (int i = 0; i < len; ++i) {
		if (arr1[i] != arr2[i]) {
			return false;
		}
	}
	return true;
}

int* createArray(int length) {
	int* a1 = new int[length];

	for (int i = 0; i < length; i++) {
		a1[i] = rand();
		//a1[i] = i + 10;
	}

	return a1;
}

int* array_copy(int* arr, int length) {
	int* a2 = new int[length];
	for (int i = 0; i < length; i++) {
		a2[i] = arr[i];
	}

	return a2;
}
//
//long testerSingleQsort(int tests, int length) {
//	double t1, t2;
//	t1 = getCPUTime();
//
//}

int getInt(const int& el) {
	return el;
}

void tester(int tests, int length) {
	double t1, t2;

	int** arrays1 = new int* [tests];
	int** arrays2 = new int* [tests];

	for (int t = 0; t < tests; ++t) {
		int* arr1 = createArray(length);
		arrays1[t] = arr1;
		arrays2[t] = array_copy(arr1, length);
		delete[] arrays1[t];
	}

	/*t1 = getCPUTime();
	for (int t = 0; t < tests; ++t) {
	 	quickSort(arrays1[t], 0, length - 1);
	}
	t2 = getCPUTime();
	
	cout << "Avg Qsort   time for " << length << " elements: " << (t2 - t1) * 1000 / tests << endl;
	*/

	int portion = catchUp = 8;
	
	t1 = getCPUTime();
	for (int t = 0; t < tests; ++t) {
		newGenerationSort(arrays2[t], length);
	}
	t2 = getCPUTime();

	cout << "Avg newSort time for " << length << " elements: " << (t2 - t1) * 1000 / tests << " Portion: "<< portion << endl;
	/*
	for (int t = 0; t < tests; ++t) {
		if (!arraysEqual(arrays1[t], arrays2[t], length)) {
			cout << "Review your code! Something wrong..." << endl;
			break;
		}
	}*/

	cout << "confirmed" << endl;	
}

int main() {
	srand(time(NULL));
	int length;
	double t1, t2;

	cout << "size: ";
	cin >> length;
	
	int t;
	cout << "tests: ";
	cin >> t;
	tester(t, length);
	system("pause");

	int* a1 = createArray(length);
	int* a2 = array_copy(a1, length);
	int* a3 = array_copy(a1, length);

	
	t1 = getCPUTime();
	quickSort(a2, 0, length - 1);
	t2 = getCPUTime();

	cout << "Qsort time: " << (t2 - t1) * 1000 << endl;
	
	t1 = getCPUTime();
	//fastSort(a1, length);
	//sort(a1, a1 + length);
	newGenerationSort(a1, length);
	t2 = getCPUTime();

	cout << "My time: " << (t2 - t1) * 1000 << endl;

	if (!arraysEqual(a1, a2, length)) {
		cout << "Something wrong with sort!" << endl;
		cout << "Source arr: ";
		printArr(a3, length);
		cout << endl;

		printArr(a1, length);
		printArr(a2, length);
		newGenerationSort(a3, length);
	}

	system("pause");
	return 0;
}

int getAvarageStep(int* arr, int len) {
	long result = 0;
	for (int i = 1; i < len; ++i) {
		result += abs(arr[i] - arr[i - 1]);
	}
	return round(result / (len - 1.));
}

// work with min and rest arrays
// 0 1 2 3 4 5 6 7 8 9 10 - indexes
// 0 0 0 0 0 1 2 3 4 5 6
// minLen = 6
// len = 11
// arr + len - minLen