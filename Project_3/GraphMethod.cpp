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

#define INF 100000000

using namespace std;

bool isValidVertex(Graph* graph, int vertex) {
    return vertex >= 0 && vertex < graph->getSize();
}

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        if (weight != other.weight) return weight < other.weight;
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

struct DisjointSet {
    vector<int> parent;
    DisjointSet(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_j] = root_i;
        }
    }
};

bool BFS(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    int size = graph->getSize();
    vector<bool> visited(size, false);
    queue<int> q;

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

        if (!first) fout << " -> ";
        fout << curr;
        first = false;

        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(curr, &adj);
        else graph->getAdjacentEdges(curr, &adj);

        for (auto const& item : adj) {
            int next_v = item.first;
            if (!visited[next_v]) {
                visited[next_v] = true;
                q.push(next_v);
            }
        }
    }
    fout << endl;
    fout << "===================" << endl << endl;
    fout.close();

    return true;
}

bool DFS(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    int size = graph->getSize();
    vector<bool> visited(size, false);
    stack<int> s;

    s.push(vertex);
    
    fout << "========DFS========" << endl;
    if (option == 'O') fout << "Directed Graph DFS" << endl;
    else fout << "Undirected Graph DFS" << endl;
    fout << "Start: " << vertex << endl;

    bool first = true;
    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        if (visited[curr]) continue;
        visited[curr] = true;

        if (!first) fout << " -> ";
        fout << curr;
        first = false;

        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(curr, &adj);
        else graph->getAdjacentEdges(curr, &adj);

        for (auto it = adj.rbegin(); it != adj.rend(); ++it) {
            int next_v = it->first;
            if (!visited[next_v]) {
                s.push(next_v);
            }
        }
    }
    fout << endl;
    fout << "===================" << endl << endl;
    fout.close();

    return true;
}

bool Kruskal(Graph* graph)
{
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<Edge> edges;

    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        graph->getAdjacentEdgesDirect(i, &adj); 
        
        for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            if (i < v) { 
                edges.push_back({i, v, w});
            }
        }
        
        adj.clear();
        graph->getAdjacentEdges(i, &adj);
         for (auto const& item : adj) {
            int v = item.first;
            int w = item.second;
            if (i < v) {
                 bool exists = false;
                 for(const auto& e : edges) {
                     if(e.u == i && e.v == v && e.weight == w) { exists = true; break; }
                 }
                 if(!exists) edges.push_back({i, v, w});
            }
        }
    }

    sort(edges.begin(), edges.end());

    DisjointSet ds(size);
    int mst_weight = 0;
    
    vector<map<int, int>> mst_adj(size);

    for (const auto& edge : edges) {
        if (ds.find(edge.u) != ds.find(edge.v)) {
            ds.unite(edge.u, edge.v);
            mst_weight += edge.weight;
            mst_adj[edge.u].insert({edge.v, edge.weight});
            mst_adj[edge.v].insert({edge.u, edge.weight});
        }
    }

    int root = ds.find(0);
    for(int i=1; i<size; i++){
        if(ds.find(i) != root) return false;
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========KRUSKAL========" << endl;
    for (int i = 0; i < size; i++) {
        fout << "[" << i << "] ";
        for (auto const& item : mst_adj[i]) {
            fout << item.first << "(" << item.second << ") ";
        }
        fout << endl;
    }
    fout << "Cost: " << mst_weight << endl;
    fout << "=======================" << endl << endl;
    fout.close();

    return true;
}

bool Dijkstra(Graph* graph, char option, int vertex)
{
    if (graph == nullptr || !isValidVertex(graph, vertex)) return false;

    int size = graph->getSize();
    
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(i, &adj);
        else graph->getAdjacentEdges(i, &adj);
        for (auto const& item : adj) {
            if (item.second < 0) return false;
        }
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    vector<int> dist(size, INF);
    vector<int> parent(size, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[vertex] = 0;
    pq.push({0, vertex});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(u, &adj);
        else graph->getAdjacentEdges(u, &adj);

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

    fout << "========DIJKSTRA========" << endl;
    if (option == 'O') fout << "Directed Graph Dijkstra" << endl;
    else fout << "Undirected Graph Dijkstra" << endl;
    fout << "Start: " << vertex << endl;

    for (int i = 0; i < size; i++) {
        fout << "[" << i << "] ";
        if (dist[i] == INF) {
            fout << "x" << endl;
        } else {
            vector<int> path;
            for (int curr = i; curr != vertex; curr = parent[curr]) {
                 path.push_back(curr);
            }
            path.push_back(vertex);
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

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex)
{
    if (graph == nullptr || !isValidVertex(graph, s_vertex) || !isValidVertex(graph, e_vertex)) return false;

    int size = graph->getSize();
    vector<int> dist(size, INF);
    vector<int> parent(size, -1);
    dist[s_vertex] = 0;

    vector<Edge> edges;
    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        if (option == 'O') graph->getAdjacentEdgesDirect(i, &adj);
        else graph->getAdjacentEdges(i, &adj);
        
        for (auto const& item : adj) {
            edges.push_back({i, item.first, item.second});
        }
    }

    for (int i = 0; i < size - 1; i++) {
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                parent[edge.v] = edge.u;
            }
        }
    }

    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            return false;
        }
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========BELLMANFORD========" << endl;
    if (option == 'O') fout << "Directed Graph Bellman-Ford" << endl;
    else fout << "Undirected Graph Bellman-Ford" << endl;

    if (dist[e_vertex] == INF) {
        fout << "x" << endl;
    } else {
        vector<int> path;
        for (int curr = e_vertex; curr != s_vertex; curr = parent[curr]) {
             if (curr == -1) break; 
             path.push_back(curr);
        }
        path.push_back(s_vertex);
        reverse(path.begin(), path.end());

        for (size_t j = 0; j < path.size(); j++) {
            if (j > 0) fout << " -> ";
            fout << path[j];
        }
        fout << endl;
        fout << "Cost: " << dist[e_vertex] << endl;
    }
    fout << "===========================" << endl << endl;
    fout.close();

    return true;
}

bool FLOYD(Graph* graph, char option)
{
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<vector<int>> dist(size, vector<int>(size, INF));

    for (int i = 0; i < size; i++) dist[i][i] = 0;

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

    for (int i = 0; i < size; i++) {
        if (dist[i][i] < 0) return false;
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========FLOYD========" << endl;
    if (option == 'O') fout << "Directed Graph Floyd" << endl;
    else fout << "Undirected Graph Floyd" << endl;

    fout << "\t";
    for(int i=0; i<size; i++) fout << "[" << i << "]" << "\t";
    fout << endl;

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

bool Centrality(Graph* graph) {
    if (graph == nullptr) return false;

    int size = graph->getSize();
    vector<vector<int>> dist(size, vector<int>(size, INF));

    for (int i = 0; i < size; i++) dist[i][i] = 0;

    for (int i = 0; i < size; i++) {
        map<int, int> adj;
        graph->getAdjacentEdges(i, &adj);

        for (auto const& item : adj) {
            if (item.second < dist[i][item.first]) dist[i][item.first] = item.second;
        }
    }

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

    for (int i = 0; i < size; i++) {
        if (dist[i][i] < 0) return false;
    }

    ofstream fout;
    fout.open("log.txt", ios::app);
    if (!fout.is_open()) return false;

    fout << "========CENTRALITY========" << endl;

    vector<double> centrality(size);
    double max_centrality = -1.0;
    
    for(int i = 0; i < size; i++) {
        long long sum_dist = 0;
        bool disconnected = false;
        for(int j = 0; j < size; j++) {
            if(i == j) continue;
            if(dist[i][j] == INF) {
                disconnected = true;
                break;
            }
            sum_dist += dist[i][j];
        }

        if(disconnected || sum_dist == 0) {
            centrality[i] = 0.0;
        } else {
            centrality[i] = (double)(size - 1) / sum_dist;
        }
        
        if (centrality[i] > max_centrality) {
            max_centrality = centrality[i];
        }
    }

    for (int i = 0; i < size; i++) {
        long long sum_dist = 0;
        bool disconnected = false;
        for (int j = 0; j < size; j++) {
            if(i == j) continue;
            if(dist[i][j] == INF) {
                disconnected = true;
                break;
            }
            sum_dist += dist[i][j];
        }

        fout << "[" << i << "] ";
        if (disconnected) {
            fout << "x" << endl;
        } else {
            fout << (size - 1) << "/" << sum_dist;
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