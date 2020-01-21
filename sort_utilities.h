#pragma once


//  new generation sort
//
//  Created by Sidelnyk Yevhen (rela589n) on 2019/12/29.
//  Copyright  © 2019 Sidelnyk Yevhen. All rights reserved.
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
			int* c = new int[lenC]; // результирующий массив
			int indx_a = 0;
			int indx_b = 0;
			int i = 0;
			
			for (; i < lenC; ++i) {
				if (indx_a < lenA) {
					if (indx_b < lenB) { // Оба массива содержат элементы
						c[i] = (a[indx_a] < b[indx_b]) ? 
							a[indx_a++] : 
							b[indx_b++];
						continue;
					} // Элементы есть только в первом
					while (indx_a < lenA)
						c[i++] = a[indx_a++];
				}
				else // Элементы есть только во втором
					while (indx_b < lenB)
						c[i++] = b[indx_b++];
				break;
			}
			
			return c;
		}

		void glueDelete(int*& arr, int*& a, int lenA, int*& b, int lenB) {
			if (lenA == 0) { // если первый пустой
				delete[] a; // высвобождаем от него память
				arr = b; // результирующий будет вторым массивом
				return;
			}
			if (lenB == 0) { // если второй пустой
				delete[] b; // высвобождаем от него память
				arr = a; // результирующий будет вторым массивом
				return;
			}

			int *copy = glue(a, lenA, b, lenB); // сливаем
			delete[]a; // удаляемо ненужные массивы
			delete[]b; // удаляемо ненужные массивы
			arr = copy;  // изменяем указатель
		}

		void newGenerationMergeSort(int* a, int lo, int hi, int& minPortion) {
			if (hi <= lo)
				return;

			int mid = lo + (hi - lo) / 2;
			if (hi - lo <= minPortion) { // если количество элементов вмещается в минимальный блок, то выполняем нашу сортировку
				int* part = glue(a + lo, hi - lo + 1, nullptr, 0); // просто копирует массив
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
			int localCatchUp = min(catchUp, len); // потому что мы не можем пытаться вставлять элемент за границами массива
			insertionSort(arr, 0, localCatchUp - 1); // для начала сортируем первые localCatchUp элементов

			if (len <= localCatchUp) // на случай если это массив на n <= catchUp элементов
				return; // а также это база рекурсии

			int* selection = new int[len << 1]; // то же что и new int[len * 2]
			int left = len - 1; // индекс для хранения новых минимальных элементов
			int right = len;  // индекс для хранения новых максимальных элементов
			
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

				if (i & 1) { // i - непарное
					restFirst[restFirstLen++] = arr[i];
				}
				else {
					restSecond[restSecondLen++] = arr[i];
				}
			}
			delete[] arr; 

			int selectionLen = right - left - 1; // просто длина нашей выборки
			int* copy = glue(selection + left + 1, selectionLen, nullptr, 0); // копируем все элементы выборки в новый массив
			
			delete[] selection; // удаляем массив размером 2 * len элементов и
			selection = copy; // вместо него используем ровно столько памяти, сколько нужно

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

			int* selection = new int[len << 1]; // то же что и new int[len * 2]
			int left = len - 1; // индекс для хранения новых минимальных элементов
			int right = len;  // индекс для хранения новых максимальных элементов

			int* restElements = new int[len];
			int restLen = 0;

			if (arr[0] > arr[1])
				swap(arr[0], arr[1]);

			selection[left--] = arr[0];
			selection[right++] = arr[1];

			for (int i = 2; i < len; ++i) {

				if (selection[right - 1] <= arr[i]) // проверяем на новый максимум
				{
					selection[right++] = arr[i];
					continue;
				}

				if (selection[left + 1] >= arr[i]) // проверяем на новый минимум
				{
					selection[left--] = arr[i];
					continue;
				}

				restElements[restLen++] = arr[i]; // если элемент не попал в выборку, он попадёт сюда
			}
			int selectionLen = right - left - 1; // длина выборки 
			int* copy = glue(selection + left + 1, selectionLen, nullptr, 0); // в даном контексте просто копирует выборку

			delete[] selection; // мы выделяли 2 * len памяти, и большая её часть в большинстве случаев просто не используется, поэтому освобождаем лишнюю память
			selection = copy; // изменяем указатель, так что теперь selection содержит только значащую информацию

			delete[] arr; // далее будет рекурсивный вызов, а все элементы для сортировки у нас уже есть, поэтому освободим память от исходного массива
		
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