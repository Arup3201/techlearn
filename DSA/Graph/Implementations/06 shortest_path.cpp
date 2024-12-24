#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void BFSModified(vector<int> adj[], int V, int source)
{
    vector<int> dists(V);
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    queue<int> q;
    q.push(source);
    visited[source] = true;
    dists[source] = 0;

    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        for (auto u : adj[v])
        {
            if (!visited[u])
            {
                q.push(u);
                dists[u] = dists[v] + 1;
                visited[u] = true;
            }
        }
    }

    for (auto x : dists)
    {
        cout << x << " ";
    }
}

int main()
{
    int V;
    V = 4;
    vector<int> adj[V];
    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 3);

    BFSModified(adj, V, 0);
    return 0;
}