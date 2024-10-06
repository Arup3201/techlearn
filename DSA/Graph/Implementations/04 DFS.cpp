#include <iostream>
#include <vector>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void DFSRec(vector<int> adj[], bool visited[], int source)
{
    cout << source << " ";
    visited[source] = true;
    for (auto v : adj[source])
    {
        if (!visited[v])
            DFSRec(adj, visited, v);
    }
}

void DFS(vector<int> adj[], int V, int source)
{
    bool visited[V];
    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
    }

    DFSRec(adj, visited, source);
}

int main()
{
    int V = 7;
    vector<int> adj[V];
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);
    addEdge(adj, 2, 3);
    addEdge(adj, 0, 4);
    addEdge(adj, 4, 5);
    addEdge(adj, 5, 6);
    addEdge(adj, 6, 4);

    DFS(adj, V, 0);
    return 0;
}