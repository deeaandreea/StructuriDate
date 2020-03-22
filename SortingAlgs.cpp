#include <iostream> 
#include <fstream> 
#include <chrono>

using namespace std::chrono; 
using namespace std;

ifstream fin("SortingAlgs.test.in");
ofstream fout("SortingAlgs.test.out");

////////////////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////////////////

// Utility function to swap two elements
void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Utility function to print an array
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

// Utility function to get maximum value in an array
int getMax(int arr[], int n)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

// Utility function to test the array is sorted
bool testSorted(int arr[], int n)
{
    for (int i = 0; i < n-1; ++i)
        if (arr[i] > arr[i+1])
            return false;
    return true;
}

// Utility function to generate an array with n random integers
// with a maximum value
int* generateArray(int n, int max)
{
    int* arr = new int[n];
    for (int i = 0; i < n; i++)
        arr[i] = rand()%max + 1;
    return arr;
}

// Utility function to initialize an array with values from another array
void copyArray(int arr[], int arr2[], int n)
{
    for (int i = 0; i < n; ++i)
        arr[i] = arr2[i];
}


////////////////////////////////////////////////////////////////////////////////
// Function implementing the Bubble Sort algorithm
////////////////////////////////////////////////////////////////////////////////

const int BUBBLE_COUNT_LIMIT = 10000;

void bubbleSort(int arr[], int n)  
{
    if (n > BUBBLE_COUNT_LIMIT)
        return;

    for (int i = 0; i < n-1; i++)
    {
        // Last i elements are already in place
        for (int j = 0; j < n-i-1; j++)
		{
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
		}
	}
}  


////////////////////////////////////////////////////////////////////////////////
// Function implementing the Count Sort algorithm
////////////////////////////////////////////////////////////////////////////////

void countSort(int arr[], int n)
{
    int max = getMax(arr, n); // max value from array
    int count[max+1]; // count array (max+1 number of elements)
	
    // Initialization of the count array
    for (int i = 0; i <= max; i++)
        count[i] = 0;
	// Store count of each integer 
    for (int i = 0; i < n; i++)
        count[arr[i]] += 1;
    // Store the sorted values in the initial array
    int pos = 0;
    for (int i = 0; i <= max; i++)
    {
        // Store in arr the number i for count[i] times
        for (int j = 1; j <= count[i]; j++)
            arr[pos++] = i;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Function implementing the Radix Sort algorithm
////////////////////////////////////////////////////////////////////////////////

// A function to do counting sort of arr[] according to
// the digit represented by exp
void countingSort(int arr[], int n, int exp) 
{
    int output[n]; // output array
    int i, count[10] = {0};

    // Store count of occurrences in count[] 
    for (i = 0; i < n; i++) 
        count[(arr[i]/exp)%10]++;

    // Change count[i] so that count[i] now contains actual 
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--)
    {
        output[count[ (arr[i]/exp)%10 ] - 1] = arr[i];
        count[(arr[i]/exp)%10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

// The main function that sorts arr[] of size n using Radix Sort
void radixSort(int arr[], int n) 
{ 
    // Find the maximum number to know number of digits 
    int m = getMax(arr, n); 
  
    // Do counting sort for every digit. Note that instead 
    // of passing digit number, exp is passed. exp is 10^i 
    // where i is current digit number 
    for (int exp = 1; m/exp > 0; exp *= 10) 
        countingSort(arr, n, exp); 
} 


////////////////////////////////////////////////////////////////////////////////
// Function implementing the Merge Sort algorithm
////////////////////////////////////////////////////////////////////////////////

// Merges two sub-arrays of arr[]: first is arr[l..m], second is arr[m+1..r]
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    // Create temp arrays
    int L[n1], R[n2]; 
  
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
	
    // Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
	// Copy the remaining elements of L[], if there are any
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[], if there are any
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
} 

// l is for left index and r is right index of the 
// sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r)
{ 
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

// Merge sort for the full array with left index 0 and right index n-1
void mergeSort(int arr[], int n)
{
    mergeSort(arr, 0, n-1);
}


////////////////////////////////////////////////////////////////////////////////
// Function implementing the Quick Sort algorithm
////////////////////////////////////////////////////////////////////////////////

// This function takes last element as pivot, places the pivot element
// at its correct position in sorted array, and places all smaller than pivot
// to left of pivot and all greater elements to right of pivot
int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // index of smaller element

    for (int j = low; j <= high - 1; j++)  
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);
}

// The main function that implements QuickSort
// arr[] --> Array to be sorted
// low --> Starting index
// high --> Ending index
void quickSort(int arr[], int low, int high)  
{  
    if (low < high)  
    {  
        // pi is partitioning index, arr[p] is now at right place
        int pi = partition(arr, low, high);
  
        // Separately sort elements before partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);  
    }  
}  

// Quick sort for the full array with left index 0 and right index n-1
void quickSort(int arr[], int n)
{
    quickSort(arr, 0, n-1);
}

////////////////////////////////////////////////////////////////////////////////
// Testing code
////////////////////////////////////////////////////////////////////////////////

void test_sort(void sort_function(int arr[], int n), int arr[], int n){
    sort_function(arr, n);
}

int runOneTest(int arr[], int n)
{
    int* arr0 = new int[n];
    copyArray(arr0, arr, n);

	auto start = high_resolution_clock::now();
    test_sort (bubbleSort, arr, n);
    auto stop = high_resolution_clock::now();
    duration<double, std::milli> duration = stop - start;
    fout << "BubbleSort: " << duration.count() << " (milliseconds)"
         << (testSorted(arr, n) ? " Sorted" : " Not sorted") << endl;

	copyArray(arr, arr0, n);
    start = high_resolution_clock::now();
	countSort(arr, n);
	stop = high_resolution_clock::now();
	duration = stop - start;
	fout << "CountSort: " << duration.count() << " (milliseconds)"
         << (testSorted(arr, n) ? " Sorted" : " Not sorted") << endl;

	copyArray(arr, arr0, n);
    start = high_resolution_clock::now();
	radixSort(arr, n);
	stop = high_resolution_clock::now();
	duration = stop - start;
    fout << "RadixSort: " << duration.count() << " (milliseconds)"
         << (testSorted(arr, n) ? " Sorted" : " Not sorted") << endl;

	copyArray(arr, arr0, n);
    start = high_resolution_clock::now();
	mergeSort(arr, 0, n-1);
	stop = high_resolution_clock::now();
	duration = stop - start;
    fout << "MergeSort: " << duration.count() << " (milliseconds)"
         << (testSorted(arr, n) ? " Sorted" : " Not sorted") << endl;

	copyArray(arr, arr0, n);
    start = high_resolution_clock::now();
	quickSort(arr, 0, n-1);
	stop = high_resolution_clock::now();
	duration = stop - start;
    fout << "QuickSort: " << duration.count() << " (milliseconds)"
         << (testSorted(arr, n) ? " Sorted" : " Not sorted") << endl;

    return 0;
}

int main()
{
    fout << "Test 0: array size = 7, max value = 90" << endl;
    int arr0[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr0)/sizeof(arr0[0]);
    runOneTest(arr0, n);
    fout << endl;

    // Read the number of tests
    int T, maxValue;
    fin >> T;

    for (int i = 1; i <= T; i++) {
        // Read n and maxValue for each test
        fin >> n >> maxValue;
  	    fout << "Test " << i << ": array size = " << n << ", max value = " << maxValue << endl;
  	    // Generate random integer values for the array
  	    int* arr = generateArray(n, maxValue);
  	    // Run the test for the generated array
  	    runOneTest(arr, n);
  	}
 	
    return 0;
}
