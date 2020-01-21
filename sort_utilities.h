#pragma once


//  new generation sort
//
//  Created by Sidelnyk Yevhen (rela589n) on 2019/12/29.
//  Copyright  � 2019 Sidelnyk Yevhen. All rights reserved.
//

namespace rela589n {
	//#include "RadixSort.h"

	namespace 
	{
		#include <iostream>
		using namespace std;

		template <class T> void swap(T& a, T& b)
		{
			T c(a); a = b; b = c;
		}

		void newGenerationSort(int*& arr, int len);
		void FirstNewGeneratingSort(int*& arr, int len);
		int* glue(int* a, int lenA, int* b, int lenB);
		void glueDelete(int*& arr, int*& a, int lenA, int*& b, int lenB);
		void merge_sort(int* a, int lo, int hi);

		int* glue(int* a, int lenA, int* b, int lenB) {
			int lenC = lenA + lenB;
			int* c = new int[lenC]; // �������������� ������
			int indx_a = 0;
			int indx_b = 0;
			int i = 0;
			
			for (; i < lenC; ++i) {
				if (indx_a < lenA) {
					if (indx_b < lenB) { // ��� ������� �������� ��������
						c[i] = (a[indx_a] < b[indx_b]) ? 
							a[indx_a++] : 
							b[indx_b++];
						continue;
					} // �������� ���� ������ � ������
					while (indx_a < lenA)
						c[i++] = a[indx_a++];
				}
				else // �������� ���� ������ �� ������
					while (indx_b < lenB)
						c[i++] = b[indx_b++];
				break;
			}
			
			return c;
		}

		void glueDelete(int*& arr, int*& a, int lenA, int*& b, int lenB) {
			if (lenA == 0) { // ���� ������ ������
				delete[] a; // ������������ �� ���� ������
				arr = b; // �������������� ����� ������ ��������
				return;
			}
			if (lenB == 0) { // ���� ������ ������
				delete[] b; // ������������ �� ���� ������
				arr = a; // �������������� ����� ������ ��������
				return;
			}

			int *copy = glue(a, lenA, b, lenB); // �������
			delete[]a; // �������� �������� �������
			delete[]b; // �������� �������� �������
			arr = copy;  // �������� ���������
		}

		void newGenerationMergeSort(int* a, int lo, int hi, int& minPortion) {
			if (hi <= lo)
				return;

			int mid = lo + (hi - lo) / 2;
			if (hi - lo <= minPortion) { // ���� ���������� ��������� ��������� � ����������� ����, �� ��������� ���� ����������
				int* part = glue(a + lo, hi - lo + 1, nullptr, 0); // ������ �������� ������
				FirstNewGeneratingSort(part, hi - lo + 1);

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

		/// works only if arr is pointer has been assigned by new keyword
		void newGenerationSort(int*& arr, int len) {
			int localCatchUp = min(catchUp, len); // ������ ��� �� �� ����� �������� ��������� ������� �� ��������� �������
			insertionSort(arr, 0, localCatchUp - 1); // ��� ������ ��������� ������ localCatchUp ���������

			if (len <= localCatchUp) // �� ������ ���� ��� ������ �� n <= catchUp ���������
				return; // � ����� ��� ���� ��������

			int* selection = new int[len << 1]; // �� �� ��� � new int[len * 2]
			int left = len - 1; // ������ ��� �������� ����� ����������� ���������
			int right = len;  // ������ ��� �������� ����� ������������ ���������
			
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

					for (int j = left; selection[j] >= selection[j + 1]; ++j)
						swap(selection[j], selection[j + 1]);

					--left;
					continue;
				}

				if (i & 1) { // i - ��������
					restFirst[restFirstLen++] = arr[i];
				}
				else {
					restSecond[restSecondLen++] = arr[i];
				}
			}
			delete[] arr; 

			int selectionLen = right - left - 1; // ������ ����� ����� �������
			int* copy = glue(selection + left + 1, selectionLen, nullptr, 0); // �������� ��� �������� ������� � ����� ������
			
			delete[] selection; // ������� ������ �������� 2 * len ��������� �
			selection = copy; // ������ ���� ���������� ����� ������� ������, ������� �����

			newGenerationSort(restFirst, restFirstLen);
			newGenerationSort(restSecond, restSecondLen);

			int* part2;
			int part2Len;
			if (selectionLen < restFirstLen) {
				glueDelete(selection, restFirst, restFirstLen, selection, selectionLen); // selection += restFirst 
				selectionLen += restFirstLen;
				
				part2 = restSecond;	
				part2Len = restSecondLen;
			}
			else {
				glueDelete(part2, restFirst, restFirstLen, restSecond, restSecondLen); // part2 = restFirst + restSecond
				part2Len = restFirstLen + restSecondLen;
			}
			
			glueDelete(arr, selection, selectionLen, part2, part2Len);
		}

		void FirstNewGeneratingSort(int*& arr, int len) {
			if (len < 2)
				return;

			int* selection = new int[len << 1]; // �� �� ��� � new int[len * 2]
			int left = len - 1; // ������ ��� �������� ����� ����������� ���������
			int right = len;  // ������ ��� �������� ����� ������������ ���������

			int* restElements = new int[len];
			int restLen = 0;

			if (arr[0] > arr[1])
				swap(arr[0], arr[1]);

			selection[left--] = arr[0];
			selection[right++] = arr[1];

			for (int i = 2; i < len; ++i) {

				if (selection[right - 1] <= arr[i]) // ��������� �� ����� ��������
				{
					selection[right++] = arr[i];
					continue;
				}

				if (selection[left + 1] >= arr[i]) // ��������� �� ����� �������
				{
					selection[left--] = arr[i];
					continue;
				}

				restElements[restLen++] = arr[i]; // ���� ������� �� ����� � �������, �� ������ ����
			}
			int selectionLen = right - left - 1; // ����� ������� 
			int* copy = glue(selection + left + 1, selectionLen, nullptr, 0); // � ����� ��������� ������ �������� �������

			delete[] selection; // �� �������� 2 * len ������, � ������� � ����� � ����������� ������� ������ �� ������������, ������� ����������� ������ ������
			selection = copy; // �������� ���������, ��� ��� ������ selection �������� ������ �������� ����������

			delete[] arr; // ����� ����� ����������� �����, � ��� �������� ��� ���������� � ��� ��� ����, ������� ��������� ������ �� ��������� �������
		
			FirstNewGeneratingSort(restElements, restLen);

			glueDelete(arr, selection, selectionLen, restElements, restLen);
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


	void newMergeSort(int *arr, int length) {
		int portion = log2(length);
		portion *= portion;
		newGenerationMergeSort(arr, 0, length - 1, portion);
	}
}