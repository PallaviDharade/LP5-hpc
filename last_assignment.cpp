#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
using namespace std;

// Euclidean distance
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

int main() {
    int n, k, iterations;

    // Dynamic input
    cout << "Enter number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);

    cout << "Enter " << n << " data points (x y):\n";
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }

    cout << "Enter number of clusters (k): ";
    cin >> k;

    if(k > n) {
        cout << "Error: k cannot be greater than number of points!\n";
        return 0;
    }

    cout << "Enter number of iterations: ";
    cin >> iterations;

    // Dynamic centroid input (IMPORTANT for viva)
    vector<double> cx(k), cy(k);
    cout << "Enter initial centroids (x y):\n";
    for(int i = 0; i < k; i++) {
        cin >> cx[i] >> cy[i];
    }

    vector<int> cluster(n);

    // K-Means iterations
    for(int iter = 0; iter < iterations; iter++) {

        // Parallel cluster assignment
        #pragma omp parallel for
        for(int i = 0; i < n; i++) {
            double minDist = 1e9;
            int idx = 0;

            for(int j = 0; j < k; j++) {
                double d = distance(x[i], y[i], cx[j], cy[j]);
                if(d < minDist) {
                    minDist = d;
                    idx = j;
                }
            }
            cluster[i] = idx;
        }

        // Reset sums
        vector<double> sumx(k, 0), sumy(k, 0);
        vector<int> count(k, 0);

        // Compute new centroids
        for(int i = 0; i < n; i++) {
            int c = cluster[i];
            sumx[c] += x[i];
            sumy[c] += y[i];
            count[c]++;
        }

        for(int j = 0; j < k; j++) {
            if(count[j] != 0) {
                cx[j] = sumx[j] / count[j];
                cy[j] = sumy[j] / count[j];
            }
        }
    }

    // Output
    cout << "\nFinal Clusters:\n";
    for(int i = 0; i < n; i++) {
        cout << "Point (" << x[i] << ", " << y[i] 
             << ") -> Cluster " << cluster[i] << endl;
    }

    return 0;
}