#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <queue>
#include <iomanip>

using namespace std;

const int V = 20;
const int INF = INT_MAX;

// Node Mapping matching Table 1.1 (A = 0 through T = 19)
string names[V] = {
    "Chitipa", "Karonga", "Rumphi", "Nkhata Bay", "Mzuzu",
    "Kasungu", "Ntchisi", "Dowa", "Salima", "Lilongwe",
    "Mchinji", "Dedza", "Ntcheu", "Mangochi", "Balaka",
    "Zomba", "Blantyre", "Mwanza", "Thyolo", "Mulanje"
};

char letters[V] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'
};

// Helper to print path using Node Letters (e.g., A -> E -> F)
void showPathLetters(int parent[], int j) {
    if (parent[j] == -1) return;
    showPathLetters(parent, parent[j]);
    cout << " -> " << letters[j];
}

// Helper to print path using District Names
void showPathNames(int parent[], int j) {
    if (parent[j] == -1) return;
    showPathNames(parent, parent[j]);
    cout << " -> " << names[j];
}

// Dijkstra's Algorithm using Priority Queue (Matches Question 3 Pseudocode)
void dijkstra(int graph[V][V], int src, bool printFullTable = false, int dest = -1) {
    int dist[V], parent[V];
    bool visited[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    // Min-heap priority queue storing {distance, vertex}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] != 0 && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Print full table matching Section 4 of the report
    if (printFullTable) {
        cout << "\n=================================================================================\n";
        cout << "SHORTEST PATHS FROM " << letters[src] << " (" << names[src] << ") TO ALL DISTRICTS\n";
        cout << "=================================================================================\n";
        cout << left << setw(6) << "Node" << setw(15) << "District" << setw(10) << "Weight" << setw(15) << "Dist (km)" << "Shortest Path\n";
        cout << "---------------------------------------------------------------------------------\n";

        for (int i = 0; i < V; i++) {
            cout << left << setw(6) << letters[i]
                 << setw(15) << names[i]
                 << setw(10) << dist[i]
                 << setw(15) << (dist[i] * 10)
                 << letters[src];
            showPathLetters(parent, i);
            cout << "\n";
        }
        cout << "=================================================================================\n";
    } 
    // Print single point-to-point route
    else if (dest != -1) {
        cout << "\nShortest Path from " << names[src] << " (" << letters[src] << ") to " 
             << names[dest] << " (" << letters[dest] << "):";
        if (dist[dest] == INF) {
            cout << "\nNo path exists.\n";
        } else {
            cout << "\nTotal Weight: " << dist[dest] << " (" << dist[dest] * 10 << " km)"
                 << "\nNode Path:    " << letters[src];
            showPathLetters(parent, dest);
            cout << "\nDistricts:    " << names[src];
            showPathNames(parent, dest);
            cout << "\n";
        }
    }
}

int main() {
    int graph[V][V] = {0};

    // Helper to add undirected edges using node letters ('A' through 'T')
    auto addEdge = [&](char uChar, char vChar, int w) {
        int u = uChar - 'A';
        int v = vChar - 'A';
        graph[u][v] = w;
        graph[v][u] = w;
    };

    // --- Adding all 40 Edges from Table 1.2 ---
    // Column 1
    addEdge('A', 'C', 10); addEdge('A', 'B', 14); addEdge('A', 'E', 21); addEdge('C', 'D', 12);
    addEdge('C', 'B', 22); addEdge('D', 'B', 16); addEdge('D', 'E', 5);  addEdge('B', 'E', 28);
    addEdge('E', 'F', 24); addEdge('G', 'F', 7);

    // Column 2
    addEdge('G', 'H', 5);  addEdge('G', 'J', 14); addEdge('H', 'F', 14); addEdge('H', 'J', 6);
    addEdge('H', 'K', 17); addEdge('F', 'I', 19); addEdge('F', 'J', 15); addEdge('F', 'K', 2);
    addEdge('I', 'J', 13); addEdge('N', 'O', 4);

    // Column 3
    addEdge('N', 'P', 10); addEdge('I', 'L', 10); addEdge('I', 'M', 20); addEdge('J', 'L', 16);
    addEdge('K', 'L', 17); addEdge('K', 'S', 18); addEdge('L', 'M', 17); addEdge('L', 'N', 26);
    addEdge('L', 'S', 9);  addEdge('M', 'N', 24);

    // Column 4
    addEdge('M', 'Q', 31); addEdge('N', 'Q', 23); addEdge('S', 'P', 5);  addEdge('S', 'R', 9);
    addEdge('O', 'P', 9);  addEdge('Q', 'P', 30); addEdge('Q', 'T', 6);  addEdge('P', 'T', 7);
    addEdge('R', 'T', 44); addEdge('S', 'O', 15);

    // 1. Verify full trace table from Chitipa (A) matching Section 4
    dijkstra(graph, 0, true);

    // 2. Specific point-to-point test cases
    cout << "\n--- Point-to-Point Test Cases ---";
    dijkstra(graph, 0, false, 16); // Chitipa (A) to Blantyre (Q) -> Expected: 83 (830 km)
    dijkstra(graph, 9, false, 16); // Lilongwe (J) to Blantyre (Q)
    dijkstra(graph, 9, false, 9);  // Lilongwe (J) to Lilongwe (J)

    return 0;
}
