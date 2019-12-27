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
		void merge_sort(int* a, int lo, int hi, int& minPortion);

		/// works only if arr is pointer assigned by new keyword
		void newGenerationSort(int*& arr, int len) {
			if (len < 2)
				return;

			int* selection = new int[len << 1];
			int left1 = len - 1;
			int right1 = len;

			int* restElements = new int[len];

			if (arr[0] > arr[1])
				swap(arr[0], arr[1]);

			selection[left1--] = arr[0];
			selection[right1++] = arr[1];

			int restLen = 0;
			for (int i = 2; i < len; ++i) {

				if (selection[right1 - 1] <= arr[i])
				{
					selection[right1++] = arr[i];
					continue;
				}

				if (selection[left1 + 1] >= arr[i])
				{
					selection[left1--] = arr[i];
					continue;
				}

				restElements[restLen++] = arr[i];
			}
			delete[] arr; // to fix memory overflow =>>>
			int selectionLen = right1 - left1 - 1;

			int* copy = glue(selection + left1 + 1, selectionLen, nullptr, 0); // return new array with the same elements
			delete[] selection; // =>>> we need to delete the rest after significant data
			selection = copy;

			newGenerationSort(restElements, restLen);

			glueAndDelete(arr, selection, selectionLen, restElements, restLen);
		}

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

		void merge_sort(int* a, int lo, int hi, int& minPortion) {
			if (hi <= lo)
				return;

			int mid = lo + (hi - lo) / 2;
			if (hi - lo <= minPortion) {
				int* part = glue(a + lo, hi - lo + 1, nullptr, 0); // copy
				newGenerationSort(part, hi - lo + 1);

				for (int i = lo; i <= hi; ++i) {
					a[i] = part[i - lo];
				}
				delete[] part;

				return;
			}

			merge_sort(a, lo, mid, minPortion);
			merge_sort(a, mid + 1, hi, minPortion);

			int* b = glue(a + lo, mid - lo + 1, a + mid + 1, hi - mid);

			for (int i = lo; i <= hi; i++)
				a[i] = b[i - lo];
			delete[] b;
		}
	}

	void fastSort(int *arr, int length) {
		int portion = log2(length);
		portion *= portion;
		merge_sort(arr, 0, length - 1, portion);
	}
}