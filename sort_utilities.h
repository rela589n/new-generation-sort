#pragma once


//  new generation sort
//
//  Created by Sidelnyk Yevhen (rela589n) on 2019/12/29.
//  Copyright  © 2019 Sidelnyk Yevhen. All rights reserved.
//

namespace rela589n {
	#include "RadixSort.h"
	//#include <algorithm>

	namespace 
	{
		struct sortNode
		{
			int start;
			int length;
			bool order; // true = asc, false = desc
		};


		template <class T> void swap(T& a, T& b)
		{
			T c(a); a = b; b = c;
		}

		void newGenerationSort(int*& arr, int len);
		void FirstNewGeneratingSort(int*& arr, int len);
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

		void newGenerationMergeSort(int* a, int lo, int hi, int& minPortion) {
			if (hi <= lo)
				return;

			int mid = lo + (hi - lo) / 2;
			if (hi - lo <= minPortion) {
				int* part = glue(a + lo, hi - lo + 1, nullptr, 0); // copy
				FirstNewGeneratingSort(part, hi - lo + 1);
				//newGenerationSort(part, hi - lo + 1);

				for (int i = lo; i <= hi; ++i) {
					a[i] = part[i - lo];
				}
				delete[] part;

				return;
			}

			newGenerationMergeSort(a, lo, mid, minPortion);
			newGenerationMergeSort(a, mid + 1, hi, minPortion);

			int* b = glue(a + lo, mid - lo + 1, a + mid + 1, hi - mid);

			for (int i = lo; i <= hi; i++)
				a[i] = b[i - lo];
			delete[] b;
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

		int catchUp = 8;

		void insertionSort(int*& arr, int lo, int hi) {
			for (int i = lo + 1; i <= hi; ++i)
				for (int j = i; j > 0 && arr[j - 1] > arr[j]; --j)
					swap(arr[j - 1], arr[j]);
		}

		/// works only if arr is pointer assigned by new keyword
		void newGenerationSort(int*& arr, int len) {
			int localCatchUp = min(len, catchUp);
			insertionSort(arr, 0, localCatchUp - 1);

			if (len <= localCatchUp)
				return;

			int* selection = new int[len << 1];
			int left = len - 1;
			int right = len;

			selection[left--] = arr[0];
			for (int i = 1; i < localCatchUp; ++i) {
				selection[right++] = arr[i];
			}

			int restLen = len >> 1;
			int* restFirst = new int[restLen];
			int* restSecond = new int[restLen];

			int restFirstLen = 0;
			int restSecondLen = 0;

			for (int i = localCatchUp; i < len; ++i) {

				if (selection[right - localCatchUp] <= arr[i])
				{
					selection[right] = arr[i];

					for (int j = right; selection[j - 1] > selection[j]; --j)
						swap(selection[j - 1], selection[j]);

					++right;
					continue;
				}

				if (selection[left + localCatchUp] >= arr[i])
				{
					selection[left] = arr[i];

					for (int j = left; selection[j] > selection[j + 1]; ++j)
						swap(selection[j], selection[j + 1]);

					--left;
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
			delete[] selection; // =>>> we need to de	lete the rest after significant data
			selection = copy;

			newGenerationSort(restFirst, restFirstLen);
			newGenerationSort(restSecond, restSecondLen);

			int* restFull = glue(restFirst, restFirstLen, restSecond, restSecondLen);

			delete[] restFirst;
			delete[] restSecond;

			glueAndDelete(arr, selection, selectionLen, restFull, restFirstLen + restSecondLen);
		}

		void FirstNewGeneratingSort(int*& arr, int len) {
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

			FirstNewGeneratingSort(restElements, restLen);

			glueAndDelete(arr, selection, selectionLen, restElements, restLen);
		}
		
		// [min, max]
		int random(int min, int max) {
			return min + rand() % ((max + 1) - min);
		}

		void quickSort(int * arr, int b, int e)
		{
			int l = b, r = e;
			int piv = arr[random(l, r)];
			while (l <= r)	
			{
				for (; arr[l] < piv; ++l);

				for (; arr[r] > piv; --r);

				if (l <= r)
					swap(arr[l++], arr[r--]);
			}
			if (b < r)
				quickSort(arr, b, r);
			if (e > l)
				quickSort(arr, l, e);
		}

	}
	


	void fastSort(int *arr, int length) {
		//newGenerationSort(arr, length);

		//int portion = log2(length);
		//portion *= portion * portion;
		int portion = 256;
		newGenerationMergeSort(arr, 0, length - 1, portion);
	}

	/*
	std::pair<int*, int> internalMerger(rela589n::sortNode nodes[], int lo, int hi, int* arr);
	// A utility function to get maximum value in arr[] 
	int getMax(int *arr, int n)
	{
		int mx = arr[0];
		for (int i = 1; i < n; ++i)
			if (arr[i] > mx)
				mx = arr[i];
		return mx;
	}

	// A function to do counting sort of arr[] according to 
	// the digit represented by exp. 
	void countSort(int *arr, int n, int exp)
	{
		int *output = new int[n]; // output array 
		int i, count[10] = { 0 };

		// Store count of occurrences in count[] 
		for (i = 0; i < n; ++i)
			++count[(arr[i] / exp) % 10];

		// Change count[i] so that count[i] now contains actual 
		//  position of this digit in output[] 
		for (i = 1; i < 10; ++i)
			count[i] += count[i - 1];

		// Build the output array 
		for (i = n - 1; i >= 0; --i)
		{
			output[count[(arr[i] / exp) % 10] - 1] = arr[i];
			--count[(arr[i] / exp) % 10];
		}

		// Copy the output array to arr[], so that arr[] now 
		// contains sorted numbers according to current digit 
		for (i = 0; i < n; ++i)
			arr[i] = output[i];
	}

	// The main function to that sorts arr[] of size n using  
	// Radix Sort 
	void radixsort(int *arr, int n)
	{
		// Find the maximum number to know number of digits 
		int m = getMax(arr, n);

		// Do counting sort for every digit. Note that instead 
		// of passing digit number, exp is passed. exp is 10^i 
		// where i is current digit number 
		for (int exp = 1; m / exp > 0; exp *= 10)
			countSort(arr, n, exp);
	}

	int sortNodeFunctor(const sortNode& node) {
		return node.length;
	}
	
	std::pair<int*, int> internalMerger(rela589n::sortNode nodes[], int lo, int hi, int* arr) {
		if (hi == lo) {
			if (!nodes[lo].order) {
				std::reverse(arr + nodes[lo].start, arr + nodes[lo].start + nodes[lo].length);
			}

			return std::make_pair(arr + nodes[lo].start, nodes[lo].length);
		}
		
		int mid = lo + (hi - lo) / 2;
		auto first = internalMerger(nodes, lo, mid, arr);
		auto second = internalMerger(nodes, mid + 1, hi, arr);

		int *merged = glue(first.first, first.second, second.first, second.second);

		return std::make_pair(merged, first.second + second.second);
	}

	int * merger(sortNode* nodes, int nodesLen, int* arr) 
	{
		RadixSort<sortNode>::radixSort(nodes, nodesLen, sortNodeFunctor);
		auto result = internalMerger(nodes, 0, nodesLen - 1, arr);
		return result.first;
	}

	void secondNewgenerationSort(int* arr, int length)
	{
		int i = 1;
		for (; i < length && arr[i - 1] < arr[i]; ++i);

		sortNode* nodes = new sortNode[(length + 1) << 1];
		int nodesLength = 0;

		bool asc = arr[0] <= arr[1];

		int lastStart = 0;
		int lastLen = 2;
		for (int i = 2; i < length - 1; ++i) {
			bool a = arr[i - 1] <= arr[i];
			
			if (a && asc || !(a || asc)) {
				++lastLen;
			}
			else {
				nodes[nodesLength].start = lastStart;
				nodes[nodesLength].length = lastLen;
				nodes[nodesLength].order = asc;
				++nodesLength;

				lastLen = 2;
				lastStart = i;
				asc = arr[i] <= arr[i + 1];
				++i;
			}
		}

		nodes[nodesLength].start = lastStart;
		nodes[nodesLength].length = lastLen;
		nodes[nodesLength].order = asc;
		++nodesLength;

		int* merged = merger(nodes, nodesLength, arr);

		for (int i = 0; i < length; ++i) {
			arr[i] = merged[i];
		}
		delete[] nodes;
		delete[] merged;
	}
	*/
}