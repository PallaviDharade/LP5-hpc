#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int sum = 0;

    // Parallel Reduction
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];

        sum += arr[i];
    }

    double avg = (double)sum / n;

    cout << "\nResults using Parallel Reduction:\n";
    cout << "Minimum = " << min_val << endl;
    cout << "Maximum = " << max_val << endl;
    cout << "Sum = " << sum << endl;
    cout << "Average = " << avg << endl;

    return 0;
}