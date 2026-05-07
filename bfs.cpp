#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

int main() {

    int V, E;

    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    vector<vector<int>> graph(V);

    cout << "Enter edges:\n";

    for(int i = 0; i < E; i++) {

        int u, v;
        cin >> u >> v;

        if(u < V && v < V) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }

    int start;

    cout << "Enter start node: ";
    cin >> start;

    vector<bool> visited(V, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS: ";

    while(!q.empty()) {

        int node = q.front();
        q.pop();

        cout << node << " ";

        #pragma omp parallel for
        for(int i = 0; i < graph[node].size(); i++) {

            int neighbor = graph[node][i];

            if(!visited[neighbor]) {

                visited[neighbor] = true;

                #pragma omp critical
                q.push(neighbor);
            }
        }
    }

    return 0;
}
