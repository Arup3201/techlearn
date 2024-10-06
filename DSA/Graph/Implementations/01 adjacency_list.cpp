#include <iostream>
#include <vector>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void printGraph(vector<int> adj[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << i << "->";
        for (auto x : adj[i])
        {
            cout << x << " ";
        }
        cout << "\n";
    }
}

int main()
{
    int v = 4;
    vector<int> adj_list[v];
    addEdge(adj_list, 0, 1);
    addEdge(adj_list, 0, 2);
    addEdge(adj_list, 1, 2);
    addEdge(adj_list, 2, 3);

    printGraph(adj_list, v);

    return 0;
}