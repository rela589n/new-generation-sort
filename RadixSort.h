#pragma once

template<class T>
class RadixSort
{
	template<class Functor>
	static int getMaxProp(T*& arr, int len, Functor f)
	{
		int maxProp = f(arr[0]);
		for (int i = 1; i < len; ++i) {
			int farr = f(arr[i]);
			if (maxProp < farr) {
				maxProp = farr;
			}
		}

		return maxProp;
	}

	static const int digitSize = 10;

	template<class Functor>
	static void countSortByDigit(T*& arr, int len, int digit, Functor &f)
	{
		T* ans = new T[len];

		int count[digitSize] = { 0 };

		for (int i = 0; i < len; ++i) {
			++count[(f(arr[i]) / digit) % digitSize];
		}

		for (int i = 1; i < digitSize; ++i) {
			count[i] += count[i - 1];
		}

		for (int i = 0; i < len; ++i) {
			T el = arr[i];
			int inx = (f(el) / digit) % digitSize;

			ans[count[inx] - 1] = el;
			--count[inx];
		}

		for (int i = 0; i < len; ++i) {
			arr[i] = ans[i];
		}
		delete[] ans;
	}

public:

	template<class Functor>
	static void radixSort(T*& arr, int len, Functor f)
	{
		int mx = getMaxProp(arr, len, f);

		for (int digit = 1; mx / digit > 0; digit *= digitSize) {
			countSortByDigit(arr, len, digit, f);
		}
	}
};

