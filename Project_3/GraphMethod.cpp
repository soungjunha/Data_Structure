#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>
#include <cmath>
#include <climits>

#define INF 100000000

using namespace std;

// Utility function to check if vertex is valid
// Returns true if vertex is within valid range [0, graph_size)
bool isValidVertex(Graph* graph, int vertex) {
    return vertex >= 0 && vertex < graph->getSize();
}

// Edge structure for Kruskal algorithm
// Represents an undirected edge with two vertices and a weight
struct Edge {
    int u, v, weight;
    
    // Comparison operator for sorting edges
    // Sorts by weight first, then by u, then by v
    bool operator<(const Edge& other) const {
        if (weight != other.weight) return weight < other.weight;
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

// Disjoint Set (Union-Find) for Kruskal algorithm
// Used to detect cycles and manage connected components
struct DisjointSet {
    vector<int> parent;     // Parent of each element
    vector<int> rank_arr;   // Rank for union by rank optimization
    
    // Constructor: initializes n elements, each in its own set
    DisjointSet(int n) {
        parent.resize(n);
        rank_arr.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    // Find operation with path compression
    // Returns the representative (root) of the set containing i
    int find(int i) {
        if (parent[i] != i) {
            parent[i] = find(parent[i]); // Path compression
        }
        return parent[i];
    }
    
    // Union operation with union by rank
    // Merges the sets containing i and j
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            // Union by rank
            if (rank_arr[root_i] < rank_arr[root_j]) {
                parent[root_i] = root_j;
            } else if (rank_arr[root_i] > rank_arr[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank_arr[root_i]++;
            }
        }
    }
};

// BFS Algorithm - Breadth-First Search
// Traverses the graph level by level starting from the given vertex
bool BFS(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    int size = graph->getSize();
    vector<bool> visited(size, false);  // Track visited vertices
    queue<int> q;                        // Queue for BFS

    // Start BFS from the given vertex
    q.push(vertex);
    visited[vertex] = true;

    fout << "========BFS========" << endl;
    if (option == 'O') fout << "Directed Graph BFS" << endl;
    else fout << "Undirected Graph BFS" << endl;
    fout << "Start: " << vertex << endl;

    bool first = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        // Print current vertex
        if (!first) fout << " -> ";
        fout << curr;
        first = false;

        // Get adjacent vertices based on graph direction
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(curr, &adj);
        else graph->getAdjacentEdges(curr, &adj);

        // Visit neighbors in ascending order (map is already sorted by key)
        for (auto const& item : adj) {
            int next_v = item.first;
            if (!visited[next_v]) {
                visited[next_v] = true;
                q.push(next_v);
            }
        }
    }
    fout << endl;
    fout << "=======================" << endl << endl;
    fout.close();

    return true;
}

// DFS Algorithm - Depth-First Search
// Traverses the graph by going as deep as possible before backtracking
bool DFS(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    int size = graph->getSize();
    vector<bool> visited(size, false);  // Track visited vertices
    stack<int> s;                        // Stack for DFS

    s.push(vertex);
    
    fout << "========DFS========" << endl;
    if (option == 'O') fout << "Directed Graph DFS" << endl;
    else fout << "Undirected Graph DFS" << endl;
    fout << "Start: " << vertex << endl;

    bool first = true;
    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        // Skip if already visited
        if (visited[curr]) continue;
        visited[curr] = true;

        // Print current vertex
        if (!first) fout << " -> ";
        fout << curr;
        first = false;

        // Get adjacent vertices based on graph direction
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(curr, &adj);
        else graph->getAdjacentEdges(curr, &adj);

        // Push in reverse order so that smallest vertex is processed first
        for (auto it = adj.rbegin(); it != adj.rend(); ++it) {
            int next_v = it->first;
            if (!visited[next_v]) {
                s.push(next_v);
            }
        }
    }
    fout << endl;
    fout << "=======================" << endl << endl;
    fout.close();

    return true;
}

// Kruskal Algorithm - Minimum Spanning Tree
// Finds MST by greedily selecting minimum weight edges that don't create cycles
bool Kruskal(Graph* graph)
{
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<Edge> edges;
    set<pair<pair<int,int>, int>> edge_set; // To avoid duplicate edges

    // Collect all edges (treat as undirected)
    // Step 1: Collect from directed edges
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        graph->getAdjacentEdgesDirect(i, &adj);
        
        for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            int u_min = min(i, v);
            int v_max = max(i, v);
            
            // Check if edge already exists (avoid duplicates)
            if (edge_set.find({{u_min, v_max}, w}) == edge_set.end()) {
                edge_set.insert({{u_min, v_max}, w});
                edges.push_back({u_min, v_max, w});
            }
        }
    }
    
    // Step 2: Also check reverse edges for undirected treatment
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        graph->getAdjacentEdges(i, &adj);
        
        for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            int u_min = min(i, v);
            int v_max = max(i, v);
            
            if (edge_set.find({{u_min, v_max}, w}) == edge_set.end()) {
                edge_set.insert({{u_min, v_max}, w});
                edges.push_back({u_min, v_max, w});
            }
        }
    }

    // Sort edges by weight (ascending order)
    sort(edges.begin(), edges.end());

    DisjointSet ds(size);
    int mst_weight = 0;      // Total weight of MST
    int edges_count = 0;     // Number of edges in MST
    
    // MST adjacency list for output
    vector<map<int, int>> mst_adj(size);

    // Kruskal's algorithm: process edges in order of increasing weight
    for (const auto& edge : edges) {
        // If adding this edge doesn't create a cycle
        if (ds.find(edge.u) != ds.find(edge.v)) {
            ds.unite(edge.u, edge.v);
            mst_weight += edge.weight;
            edges_count++;
            // Add edge to both vertices (undirected)
            mst_adj[edge.u].insert({edge.v, edge.weight});
            mst_adj[edge.v].insert({edge.u, edge.weight});
        }
    }

    // Check if MST connects all vertices (should have size-1 edges)
    if (edges_count != size - 1) return false;

    // Output MST to file
    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========KRUSKAL========" << endl;
    for (int i = 0; i < size; i++) {
        fout << "[" << i << "]";
        for (auto const& item : mst_adj[i]) {
            fout << " " << item.first << "(" << item.second << ")";
        }
        fout << endl;
    }
    fout << "Cost: " << mst_weight << endl;
    fout << "=======================" << endl << endl;
    fout.close();

    return true;
}

// Dijkstra Algorithm - Single Source Shortest Path (no negative weights)
// Uses priority queue to find shortest paths from source to all vertices
bool Dijkstra(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    int size = graph->getSize();
    
    // Check for negative weights (Dijkstra doesn't work with negative weights)
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(i, &adj);
        else graph->getAdjacentEdges(i, &adj);
        for (auto const& item : adj) {
            if (item.second < 0) return false;
        }
    }

    vector<long long> dist(size, INF);  // Distance from source to each vertex
    vector<int> parent(size, -1);        // Parent of each vertex in shortest path tree
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    // Initialize source vertex
    dist[vertex] = 0;
    pq.push({0, vertex});

    // Dijkstra's algorithm main loop
    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Skip if we've already found a shorter path
        if (d > dist[u]) continue;

        // Get adjacent vertices
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(u, &adj);
        else graph->getAdjacentEdges(u, &adj);

        // Relax edges
        for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Output results
    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========DIJKSTRA========" << endl;
    if (option == 'O') fout << "Directed Graph Dijkstra" << endl;
    else fout << "Undirected Graph Dijkstra" << endl;
    fout << "Start: " << vertex << endl;

    // Print shortest path to each vertex
    for (int i = 0; i < size; i++) {
        fout << "[" << i << "] ";
        if (dist[i] == INF) {
            fout << "x" << endl;
        } else {
            // Reconstruct path from source to i
            vector<int> path;
            int curr = i;
            while (curr != -1) {
                path.push_back(curr);
                if (curr == vertex) break;
                curr = parent[curr];
            }
            reverse(path.begin(), path.end());
            
            for (size_t j = 0; j < path.size(); j++) {
                if (j > 0) fout << " -> ";
                fout << path[j];
            }
            fout << " (" << dist[i] << ")" << endl;
        }
    }
    fout << "========================" << endl << endl;
    fout.close();

    return true;
}

// Bellman-Ford Algorithm - Single Source Shortest Path (handles negative weights)
// Can detect negative cycles and works with negative edge weights
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex)
{
    if (graph == nullptr || !isValidVertex(graph, s_vertex) || !isValidVertex(graph, e_vertex)) return false;

    int size = graph->getSize();
    vector<long long> dist(size, INF);  // Distance from source to each vertex
    vector<int> parent(size, -1);        // Parent of each vertex in shortest path tree
    dist[s_vertex] = 0;

    // Collect all edges based on direction option
    vector<Edge> edges;
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        if (option == 'O') {
            graph->getAdjacentEdgesDirect(i, &adj);
        } else {
            graph->getAdjacentEdges(i, &adj);
        }
        
        for (auto const& item : adj) {
            edges.push_back({i, item.first, item.second});
        }
    }

    // Relax edges |V| - 1 times (Bellman-Ford main loop)
    for (int i = 0; i < size - 1; i++) {
        bool updated = false;
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                parent[edge.v] = edge.u;
                updated = true;
            }
        }
        if (!updated) break; // Early termination if no updates
    }

    // Check for negative cycle
    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            return false; // Negative cycle detected
        }
    }

    // Output results
    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========BELLMANFORD========" << endl;
    if (option == 'O') fout << "Directed Graph Bellman-Ford" << endl;
    else fout << "Undirected Graph Bellman-Ford" << endl;

    if (dist[e_vertex] == INF) {
        fout << "x" << endl;
    } else {
        // Reconstruct path from source to destination
        vector<int> path;
        int curr = e_vertex;
        while (curr != -1) {
            path.push_back(curr);
            if (curr == s_vertex) break;
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        for (size_t j = 0; j < path.size(); j++) {
            if (j > 0) fout << " -> ";
            fout << path[j];
        }
        fout << endl;
        fout << "Cost: " << dist[e_vertex] << endl;
    }
    fout << "============================" << endl << endl;
    fout.close();

    return true;
}

// Floyd-Warshall Algorithm - All Pairs Shortest Path
// Computes shortest paths between all pairs of vertices
bool FLOYD(Graph* graph, char option)
{
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<vector<long long>> dist(size, vector<long long>(size, INF));

    // Initialize diagonal to 0 (distance from vertex to itself)
    for (int i = 0; i < size; i++) dist[i][i] = 0;

    // Initialize with edge weights
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(i, &adj);
        else graph->getAdjacentEdges(i, &adj);

        for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            if (w < dist[i][v]) dist[i][v] = w;
        }
    }

    // Floyd-Warshall main loop
    // For each intermediate vertex k
    for (int k = 0; k < size; k++) {
        // For each source vertex i
        for (int i = 0; i < size; i++) {
            // For each destination vertex j
            for (int j = 0; j < size; j++) {
                // If path through k is shorter, update distance
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    // Check for negative cycle (diagonal should remain 0)
    for (int i = 0; i < size; i++) {
        if (dist[i][i] < 0) return false;
    }

    // Output results
    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========FLOYD========" << endl;
    if (option == 'O') fout << "Directed Graph Floyd" << endl;
    else fout << "Undirected Graph Floyd" << endl;

    // Print column headers
    fout << "\t";
    for (int i = 0; i < size; i++) fout << "[" << i << "]" << "\t";
    fout << endl;

    // Print distance matrix
    for (int i = 0; i < size; i++) {
        fout << "[" << i << "]" << "\t";
        for (int j = 0; j < size; j++) {
            if (dist[i][j] == INF) fout << "x" << "\t";
            else fout << dist[i][j] << "\t";
        }
        fout << endl;
    }
    fout << "=====================" << endl << endl;
    fout.close();

    return true;
}

// Closeness Centrality - Graph centrality measure (undirected, weighted)
// Measures how central each vertex is based on its distance to all other vertices
bool Centrality(Graph* graph) {
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<vector<long long>> dist(size, vector<long long>(size, INF));

    // Initialize diagonal to 0
    for (int i = 0; i < size; i++) dist[i][i] = 0;

    // Initialize with edge weights (undirected - use getAdjacentEdges)
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        graph->getAdjacentEdges(i, &adj);

        for (auto const& item : adj) {
            if (item.second < dist[i][item.first]) {
                dist[i][item.first] = item.second;
            }
        }
    }

    // Floyd-Warshall for all-pairs shortest paths
    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    // Check for negative cycle
    for (int i = 0; i < size; i++) {
        if (dist[i][i] < 0) return false;
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========CENTRALITY========" << endl;

    // Calculate centrality for each vertex
    // Centrality(v) = (n-1) / sum of distances from v to all other vertices
    vector<double> centrality(size);
    vector<long long> sum_dist(size);
    vector<bool> disconnected(size, false);
    double max_centrality = -1.0;

    for (int i = 0; i < size; i++) {
        sum_dist[i] = 0;
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (dist[i][j] == INF) {
                disconnected[i] = true;
                break;
            }
            sum_dist[i] += dist[i][j];
        }

        if (disconnected[i] || sum_dist[i] == 0) {
            centrality[i] = 0.0;
        } else {
            centrality[i] = (double)(size - 1) / sum_dist[i];
        }
        
        // Track maximum centrality
        if (!disconnected[i] && centrality[i] > max_centrality) {
            max_centrality = centrality[i];
        }
    }

    // Output results
    for (int i = 0; i < size; i++) {
        fout << "[" << i << "] ";
        if (disconnected[i]) {
            fout << "x" << endl;
        } else {
            // Output as fraction (n-1)/sum
            fout << (size - 1) << "/" << sum_dist[i];
            // Mark vertex with highest centrality
            if (abs(centrality[i] - max_centrality) < 1e-9 && max_centrality > 0) {
                fout << " <- Most Central";
            }
            fout << endl;
        }
    }

    fout << "==========================" << endl << endl;
    fout.close();

    return true;
}