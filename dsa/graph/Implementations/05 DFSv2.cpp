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

void DFSDis(vector<int> adj[], int V, int source)
{
    bool visited[V];
    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
    }

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            DFSRec(adj, visited, i);
        }
    }
}

int main()
{
    int V = 5;
    vector<int> adj[V];
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);
    addEdge(adj, 3, 4);

    DFSDis(adj, V, 0);
    return 0;
}