#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// ---------------- Bubble Sort ----------------
void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Parallel Bubble Sort (Odd-Even)
void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// ---------------- Merge Sort ----------------
void merge(vector<int> &arr, int l, int m, int r) {
    vector<int> left(arr.begin()+l, arr.begin()+m+1);
    vector<int> right(arr.begin()+m+1, arr.begin()+r+1);

    int i=0, j=0, k=l;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(vector<int> &arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int> &arr, int l, int r, int depth=0) {
    if (l < r) {
        int m = (l + r) / 2;

        if (depth < 3) { // limit threads
            #pragma omp task
            parallelMergeSort(arr, l, m, depth+1);

            #pragma omp task
            parallelMergeSort(arr, m+1, r, depth+1);

            #pragma omp taskwait
        } else {
            mergeSort(arr, l, m);
            mergeSort(arr, m+1, r);
        }

        merge(arr, l, m, r);
    }
}

// ---------------- Main ----------------
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    vector<int> temp;

    // -------- Bubble Sort --------
    temp = arr;
    double start = omp_get_wtime();
    bubbleSort(temp);
    double end = omp_get_wtime();
    cout << "\nSequential Bubble Sort Time: " << (end - start) << " sec";

    temp = arr;
    start = omp_get_wtime();
    parallelBubbleSort(temp);
    end = omp_get_wtime();
    cout << "\nParallel Bubble Sort Time: " << (end - start) << " sec";

    // -------- Merge Sort --------
    temp = arr;
    start = omp_get_wtime();
    mergeSort(temp, 0, n-1);
    end = omp_get_wtime();
    cout << "\nSequential Merge Sort Time: " << (end - start) << " sec";

    temp = arr;
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(temp, 0, n-1);
    }

    end = omp_get_wtime();
    cout << "\nParallel Merge Sort Time: " << (end - start) << " sec";

    return 0;
}

// g++ -fopenmp parallel_sorting.cpp -o parallel_sorting
// ./parallel_sorting