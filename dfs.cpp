#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

vector<vector<int>> graph;
vector<bool> visited;

void dfs(int node) {

    visited[node] = true;

    cout << node << " ";

    #pragma omp parallel for
    for(int i = 0; i < graph[node].size(); i++) {

        int neighbor = graph[node][i];

        if(!visited[neighbor]) {
            dfs(neighbor);
        }
    }
}

int main() {

    int V, E;

    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    graph.resize(V);
    visited.resize(V, false);

    cout << "Enter edges:\n";

    for(int i = 0; i < E; i++) {

        int u, v;
        cin >> u >> v;

        // Safe check
        if(u < V && v < V) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }

    int start;

    cout << "Enter start node: ";
    cin >> start;

    cout << "DFS: ";

    dfs(start);

    return 0;
}
