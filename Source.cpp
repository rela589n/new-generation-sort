#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include "time_utilities.h"
#include "sort_utilities.h"

using namespace std;
using namespace rela589n;

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
		//a1[i] = rand();
		a1[i] = (i + 1);
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

void tester(int tests, int length) {
	double t1, t2;

	int** arrays1 = new int* [tests];
	int** arrays2 = new int* [tests];

	for (int t = 0; t < tests; ++t) { // просто заполнение массивов
		int* arr1 = createArray(length);
		arrays1[t] = arr1;
		arrays2[t] = array_copy(arr1, length);
	}

	t1 = getCPUTime();
	for (int t = 0; t < tests; ++t) {
	 	quickSort(arrays1[t], 0, length - 1);
	}
	t2 = getCPUTime();
	
	cout << "Avg Qsort       time for " << length << " elements: " << (t2 - t1) * 1000 / tests << endl;
	
	int portion = catchUp = 8;
	
	t1 = getCPUTime();
	for (int t = 0; t < tests; ++t) {
		newGenerationSort(arrays2[t], length);
	}	
	t2 = getCPUTime();

	cout << "Avg newGenSort  time for " << length << " elements: " << (t2 - t1) * 1000 / tests //<< " Catch up coef: "<< portion
		<< endl;

	bool confirmed = true; // проверяем идентичны ли массивы
	for (int t = 0; t < tests; ++t) {
		if (!arraysEqual(arrays1[t], arrays2[t], length)) {
			confirmed = false;
			break;
		}
	}
	if (confirmed) {
		cout << "Confirmed" << endl;
	}
	else {
		cout << "Review your code! Something wrong..." << endl;
	}
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

	return 0;
}
// work with min and rest arrays
// 0 1 2 3 4 5 6 7 8 9 10 - indexes
// 0 0 0 0 0 1 2 3 4 5 6
// minLen = 6
// len = 11
// arr + len - minLen
