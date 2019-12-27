#include <iostream>
#include <ctime>
#include <algorithm>
#include "time_utilities.h"
#include "sort_utilities.h"

using namespace std;

int getAvarageStep(int* arr, int len);

void printArr(int* arr, int len) {
	for (int i = 0; i < len; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

bool arrsEqual(int* arr1, int* arr2, int len) {
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

int main() {
	srand(time(NULL));
	int length;
	double t1, t2;

	cout << "size: ";
	cin >> length;

	int* a1 = createArray(length);
	int* a2 = array_copy(a1, length);
	int* a3 = array_copy(a1, length);

	t1 = getCPUTime();
	sort(a2, a2 + length);
	t2 = getCPUTime();

	cout << "Qsort time: " << (t2 - t1) * 1000 << endl;
	
	t1 = getCPUTime();
	rela589n::fastSort(a1, length);
	t2 = getCPUTime();

	cout << "My time: " << (t2 - t1) * 1000 << endl;

	if (!arrsEqual(a1, a2, length)) {
		cout << "Something wrong with sort!" << endl;
		cout << "Source arr: ";
		printArr(a3, length);
		cout << endl;

		printArr(a1, length);
		printArr(a2, length);
		
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