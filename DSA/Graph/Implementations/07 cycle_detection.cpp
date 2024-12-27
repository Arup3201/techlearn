#include <iostream>
#include <vector>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

bool DFSRec(vector<int> adj[], bool visited[], int source, int parent)
{
    visited[source] = true;

    for (auto u : adj[source])
    {
        if (!visited[u])
        {
            if (DFSRec(adj, visited, u, source))
                return true;
        }
        else if (u != parent)
            return true;
    }

    return false;
}

bool DFS(vector<int> adj[], int V, int source)
{
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            if (DFSRec(adj, visited, i, -1))
            {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int V = 4;
    vector<int> adj[V];
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);
    addEdge(adj, 2, 3);
    addEdge(adj, 1, 3);

    cout << (DFS(adj, V, 0) ? "Yes" : "No");

    return 0;
}