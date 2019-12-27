#include <iostream>
#include <ctime>
#include <algorithm>

#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unable to define getCPUTime( ) for an unknown OS."
#endif

/**
 * Returns the amount of CPU time used by the current process,
 * in seconds, or -1.0 if an error occurred.
 */
double getCPUTime()
{
#if defined(_WIN32)
	/* Windows -------------------------------------------------- */
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if (GetProcessTimes(GetCurrentProcess(),
		&createTime, &exitTime, &kernelTime, &userTime) != -1)
	{
		SYSTEMTIME userSystemTime;
		if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1)
			return (double)userSystemTime.wHour * 3600.0 +
			(double)userSystemTime.wMinute * 60.0 +
			(double)userSystemTime.wSecond +
			(double)userSystemTime.wMilliseconds / 1000.0;
	}

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
	/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */

#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
	/* Prefer high-res POSIX timers, when available. */
	{
		clockid_t id;
		struct timespec ts;
#if _POSIX_CPUTIME > 0
		/* Clock ids vary by OS.  Query the id, if possible. */
		if (clock_getcpuclockid(0, &id) == -1)
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
			/* Use known clock id for AIX, Linux, or Solaris. */
			id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
			/* Use known clock id for BSD or HP-UX. */
			id = CLOCK_VIRTUAL;
#else
			id = (clockid_t)-1;
#endif
		if (id != (clockid_t)-1 && clock_gettime(id, &ts) != -1)
			return (double)ts.tv_sec +
			(double)ts.tv_nsec / 1000000000.0;
	}
#endif

#if defined(RUSAGE_SELF)
	{
		struct rusage rusage;
		if (getrusage(RUSAGE_SELF, &rusage) != -1)
			return (double)rusage.ru_utime.tv_sec +
			(double)rusage.ru_utime.tv_usec / 1000000.0;
	}
#endif

#if defined(_SC_CLK_TCK)
	{
		const double ticks = (double)sysconf(_SC_CLK_TCK);
		struct tms tms;
		if (times(&tms) != (clock_t)-1)
			return (double)tms.tms_utime / ticks;
	}
#endif

#if defined(CLOCKS_PER_SEC)
	{
		clock_t cl = clock();
		if (cl != (clock_t)-1)
			return (double)cl / (double)CLOCKS_PER_SEC;
	}
#endif

#endif

	return -1;      /* Failed. */
}


using namespace std;

int* glue(int* a, int lenA, int* b, int lenB);
void glueAndDelete(int*& arr, int*& a, int lenA, int*& b, int lenB);
void newGenerationSort(int*& arr, int len, int avgStep);
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

int main() {
	srand(time(NULL));
	int length, i;
	cout << "size: ";
	cin >> length;

	int* a1 = new int[length];
	int* a2 = new int[length];
	int* a3 = new int[length];

	for (i = 0; i < length; i++) {
		//cin >> a[i];
		a1[i] = rand();
		a2[i] = a1[i];
		a3[i] = a1[i];
		//cout << a[i] << '\t';
	}

	double t1 = getCPUTime();
	sort(a2, a2 + length);
	double t2 = getCPUTime();

	cout << "Qsort time: " << (t2 - t1) * 1000 << endl;

	t1 = getCPUTime();
	newGenerationSort(a1, length, getAvarageStep(a1, length) << 3);
	t2 = getCPUTime();

	cout << "My time: " << (t2 - t1) * 1000 << endl;

	if (!arrsEqual(a1, a2, length)) {
		cout << "Something wrong with sort!" << endl;
		cout << "Source arr: ";
		printArr(a3, length);
		cout << endl;

		printArr(a1, length);
		printArr(a2, length);
		newGenerationSort(a3, length, getAvarageStep(a3, length));
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

void newGenerationSort(int*& arr, int len, int avgStep) {
	if (len < 2)
		return;
	
	int* selection1 = new int[len << 1];
	int left1 = len - 1;
	int right1 = len;

	int* restElements = new int[len];

	if (arr[0] > arr[1])
		swap(arr[0], arr[1]);

	selection1[left1--] = arr[0];
	selection1[right1++] = arr[1];

	int restLen = 0;
	for (int i = 2; i < len; ++i) {

		if (abs(selection1[right1 - 1] - arr[i]) <= avgStep && 
			selection1[right1 - 1] <= arr[i]) 
		{
			selection1[right1++] = arr[i];
			continue;
		}

		if (abs(selection1[left1 + 1] - arr[i]) && 
			selection1[left1 + 1] >= arr[i]) 
		{
			selection1[left1--] = arr[i];
			continue;
		}

		restElements[restLen++] = arr[i];
	}
	delete[] arr; // to fix memory overflow =>>>
	int firstLen = right1 - left1 - 1;
	
	int* copy = glue(selection1 + left1 + 1, firstLen, nullptr, 0); // return new array with the same elements
	delete[] selection1; // =>>> we need to delete the rest after significant data
	selection1 = copy;

	newGenerationSort(restElements, restLen, avgStep);

	glueAndDelete(arr, selection1, firstLen, restElements, restLen); 
}

// work with min and rest arrays
// 0 1 2 3 4 5 6 7 8 9 10 - indexes
// 0 0 0 0 0 1 2 3 4 5 6
// minLen = 6
// len = 11
// arr + len - minLen

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