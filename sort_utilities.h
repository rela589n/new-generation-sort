#pragma once

namespace rela589n {
	namespace 
	{
		template <class T> void swap(T& a, T& b)
		{
			T c(a); a = b; b = c;
		}

		void newGenerationSort(int*& arr, int len);
		int* glue(int* a, int lenA, int* b, int lenB);
		void glueAndDelete(int*& arr, int*& a, int lenA, int*& b, int lenB);
		void merge_sort(int* a, int lo, int hi);

		int* glue(int* a, int lenA, int* b, int lenB) {
			int lenC = lenA + lenB, * c = new int[lenC], indx_a = 0, indx_b = 0, i = 0;
			for (; i < lenC; i++) {
				if (indx_a < lenA) {
					if (indx_b < lenB) {
						c[i] = (a[indx_a] < b[indx_b]) ? a[indx_a++] : b[indx_b++];
						continue;
					}
					while (indx_a < lenA)
						c[i++] = a[indx_a++];
				}
				else
					while (indx_b < lenB)
						c[i++] = b[indx_b++];
				break;
			}
			return c;
		}

		void glueAndDelete(int*& arr, int*& a, int lenA, int*& b, int lenB) {
			if (lenA == 0) {
				arr = b;
				delete[] a;
				return;
			}
			if (lenB == 0) {
				arr = a;
				delete[] b;
				return;
			}

			arr = glue(a, lenA, b, lenB);
			delete[]a;
			delete[]b;
		}

		void merge_sort(int* a, int lo, int hi) {
			if (hi <= lo)
				return;

			int mid = lo + (hi - lo) / 2;
			//if (hi - lo <= minPortion) {
			//	int* part = glue(a + lo, hi - lo + 1, nullptr, 0); // copy
			//	newGenerationSort(part, hi - lo + 1);

			//	for (int i = lo; i <= hi; ++i) {
			//		a[i] = part[i - lo];
			//	}
			//	delete[] part;

			//	return;
			//}

			merge_sort(a, lo, mid);
			merge_sort(a, mid + 1, hi);

			int* b = glue(a + lo, mid - lo + 1, a + mid + 1, hi - mid);

			for (int i = lo; i <= hi; i++)
				a[i] = b[i - lo];
			delete[] b;
		}

		/// works only if arr is pointer assigned by new keyword
		void newGenerationSort(int*& arr, int len) {
			if (len < 2)
				return;

			int* selection = new int[len << 1];
			int left = len - 1;
			int right = len;

			if (arr[0] > arr[1])
				swap(arr[0], arr[1]);

			selection[left--] = arr[0];
			selection[right++] = arr[1];

			int restLen = len >> 1;
			int* restFirst = new int[restLen];
			int* restSecond = new int[restLen];

			int restFirstLen = 0;
			int restSecondLen = 0;

			for (int i = 2; i < len; ++i) {

				if (selection[right - 1] <= arr[i])
				{
					selection[right++] = arr[i];
					continue;
				}

				if (selection[left + 1] >= arr[i])
				{
					selection[left--] = arr[i];
					continue;
				}

				bool flag = false;

				/*if (restFirstLen < restLen) {
					if (!restFirstLen) {
						flag = true;
					}
					else if (restSecondLen < restLen && restSecondLen) {
						flag = restFirst[restFirstLen - 1] <= arr[i];
					}
					else {
						flag = true;
					}
				}*/
				
				flag = i & 1;
				if (flag) {
					restFirst[restFirstLen++] = arr[i];
				}
				else {
					restSecond[restSecondLen++] = arr[i];
				}
			}
			delete[] arr; // to fix memory overflow =>>>
			int selectionLen = right - left - 1;

			int* copy = glue(selection + left + 1, selectionLen, nullptr, 0); // return new array with the same elements
			delete[] selection; // =>>> we need to delete the rest after significant data
			selection = copy;

			newGenerationSort(restFirst, restFirstLen);
			newGenerationSort(restSecond, restSecondLen);

			int* restFull = glue(restFirst, restFirstLen, restSecond, restSecondLen);

			delete[] restFirst;
			delete[] restSecond;

			glueAndDelete(arr, selection, selectionLen, restFull, restFirstLen + restSecondLen);
		}


		/* This function takes last element as pivot, places
		   the pivot element at its correct position in sorted
			array, and places all smaller (smaller than pivot)
		   to left of pivot and all greater elements to right
		   of pivot */
		int partition(int arr[], int low, int high)
		{
			int pivot = arr[high];    // pivot 
			int i = (low - 1);  // Index of smaller element 

			for (int j = low; j <= high - 1; j++)
			{
				// If current element is smaller than or 
				// equal to pivot 
				if (arr[j] <= pivot)
				{
					i++;    // increment index of smaller element 
					swap(arr[i], arr[j]);
				}
			}
			swap(arr[i + 1], arr[high]);
			return (i + 1);
		}

		/* The main function that implements QuickSort
		 arr[] --> Array to be sorted,
		  low  --> Starting index,
		  high  --> Ending index */
		void quickSort(int arr[], int low, int high)
		{
			if (low < high)
			{
				/* pi is partitioning index, arr[p] is now
				   at right place */
				int pi = partition(arr, low, high);

				// Separately sort elements before 
				// partition and after partition 
				quickSort(arr, low, pi - 1);
				quickSort(arr, pi + 1, high);
			}
		}
	}


	void fastSort(int *arr, int length) {
		//newGenerationSort(arr, length);

		//int portion = log2(length);
		//portion *= portion * portion;
		merge_sort(arr, 0, length - 1);
	}
}