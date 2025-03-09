#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void BFS(vector<int> adj[], int source, bool visited[])
{
    queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        cout << u << " ";

        for (auto v : adj[u])
        {
            if (!visited[v])
            {
                q.push(v);
                visited[v] = true;
            }
        }
    }
}

void BFSDis(vector<int> adj[], int V)
{
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            BFS(adj, i, visited);
        }
    }
}

int main()
{
    int V = 7;
    vector<int> adj[V];
    // Connected graph 1
    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 3);
    // Connected graph 2
    addEdge(adj, 4, 5);
    addEdge(adj, 4, 6);
    addEdge(adj, 5, 6);

    BFSDis(adj, V);
    return 0;
}