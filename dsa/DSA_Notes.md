# Data Structures and Algorithms

Data Structures and algorithms are one of the most important topics in computer science where you learn to solve problems with optimized algorithms that take as little time as possible. It is the study to making algorithms and thinking of optimized solutions that takes less memory and less time.

This note will covers the following topics-

1. Mathematics
2. Bit Manipulation
3. Recursion
4. Arrays
5. Searching
6. Sorting
7. Matrix
8. Hashing
9. Strings
10. LinkedList
11. Stack
12. Queue
13. Deque
14. Tree
15. Binary Search Tree
16. Heap
17. Graph
18. Greedy
19. Backtracking
20. Dynamic Programming
21. Trie
22. Segment and Binary Indexed Tree
23. Disjoint Set

## 6. Sorting

In this section, I will discuss about the sorting techniques. Each technique is used in certain situations and gives good result when used. Some of them has higher time complexity some of them has lower.

But, there are 2 factors that you will hear frequently when going through the sorting algorithms - Inplace, Stable.

- Inplace - sorting is performed on the original array or new sorted array is returned.
- Stable - Same values are ordered in the exact previous order or not.

### Insertion Sort

Insertion sort is a stable sort and an inplace algorithm that changes the original array without returning any sorted array.

The best way to describe this algorithm is by card shifting. When you have cards in your hand and you have some way to know which comes before the other one. You go through the cards from left to right. When you encounter a card that is smaller than the previous card, you start shifting the bigger cards to the right of this smaller card untill you find a card that is same or less than the smaller card on it's left side.

### Merge Sort

Merge sort has the following properties -

1. It follows divide and conquer algorithm (Divide, Conquer & Merge).
2. Stable Algorithm - it maintains the original order of equal items.
3. Works in $\theta(nlogn)$ which is the best time you can get on a single processor for a random input. But, it takes $O(n)$ extra space. There variants of merge sort that optimize the typical merge sort that is block merge sort.
4. It is well suited for external sorting - we can bring parts of a whole input and then sort them using merge sort, by sorting parts of the input one by one we can sort the whole array - we don't need the whole array in RAM.
5. For Linked List, merge sort works in $O(1)$ aux space, for array quick sort works better than merge sort.

**merge two sorted arrays**

```c

```

## 9. Strings

### Suffix Array

#### What is a suffix array.

Consider a string `s`.

![String s](./Strings/images/string_s.png)

Let's write all its suffixes in lexicographical order. We get this an array of strings.

![Suffix array of s](./Strings/images/suffix_array.png)

This sequence of suffixes is called the `suffix array`. How will we store it? If you store it as strings, then it will occupy $O(n^2)$ memory. To keep it smaller, let's notice that the suffix can be identified by the index of the first character. In our example, suffixes will have these numbers.

![Suffix array as integer numbers](./Strings/images/suffix_array_as_integers.png)

We will store the suffix array as an array `p` of suffix numbers in sorted order. Then it will occupy $O(n)$ memory. Now our first task is this: given the string `s`, build an array `p`.

To make the algorithm simpler, we will make some preparations. First, add the symbol `$` to the end of the string. This will be a special character that is smaller than all characters in the string. As a result, the symbol `$` will be added to each suffix. Let's notice that the order of the strings has not changed, because `$` is smaller than all characters.

![$ added to the string](./Strings/images/suffix_array_$_added.png)

Now let's make the length of all the strings the same. To do this, let's write after `$` all the other characters in the string in a cycle. Again, note that order of strings has not changed, because the characters after `$` do not affect the string ordering.

![Characters added in cycle to make same length](./Strings/images/suffix_array_$_same_length.png)

Now the last preparation. Let's make the length of the strings the power of two, for this we add more characters in a cycle until the length of the string becomes a power of two. In our example, the string length is 7, so we need to add one character.

![Characters added to make the length power of 2](./Strings/images/suffix_array_$_powerof2.png)

#### Algorithm Idea

We will build this table by columns, each time increasing the number of columns twice. That is, first we construct the first column, then the first two, then four, and so on. In other words, we divide the algorithm into $logn$ iterations, and on the iteration `k` we construct an array consisting of the strings $s[i..i+2^k−1]$, sorted in lexicographical order (we assume that the string is cyclic, that is, the character with index i corresponds to the character with index `i mod n` of the original string).

Let's start from the base, if `k=0`, then we need to sort the strings of one character $s[i..i]$. This can be done by any sort in $O(nlogn)$.

Now make the transition from `k` to `k+1`. We need to sort the strings of length $2^{k+1}$, using the order of the strings of length $2^k$. To do this, let's learn how to quickly compare strings of length $2^{k+1}$. Suppose we have two strings **A** and **B**, both have lengths $2^{k+1}$. Let's divide both strings into two halves, then each half has a length $2^k$.

Let's compare their left halves. If they are not equal, for example, if `A.left` is less than `B.left`. This means that they have some common prefix, and after it comes a character that is not equal, and this character in the string **A** is less than in the string **B**. In this case, the entire string **A** is less than the string **B**, and the right halves should not be compared.

![Suffix array left left side less](./Strings/images/suffix_array_left_less.png)

If the left halves are equal, then let's compare the right halves. Suppose, for example, `A.right` is less than `B.right`. This again means that there is a common prefix, and then a character that is less in the string **A** than in the string **B**. Again we get that the string **A** is less than the string **B**.

![Suffix array right side less](./Strings/images/suffix_array_right_less.png)

Thus, we get that `A<B⟺A.left<B.left OR (A.left=B.left AND A.right<B.right)`. If we learn to quickly compare strings of length $2^k$, then we will make a comparator that quickly compares strings of length $2^{k+1}$.

Now we need to learn how to compare strings of length $2^k$ quickly. We will do it as follows. Let's go through the strings of length $2^k$ in sorted order, and assign them integer numbers so that a smaller string corresponds to a smaller number. We call these numbers **equivalence classes**. Now, instead of comparing strings, we will compare their equivalence classes. Thus, we can compare strings of length $2^{k+1}$ in $O(1)$.

For example, for the string that we analyzed above, for `k=1` we get the following order of strings of length 2. Let's go through these strings and assign them equivalence classes (array **c**).

![Suffix array equivalence class](./Strings/images/suffix_array_equivalence_class.png)

Now, if, for example, we want to compare the strings abab and abba. Divide them into two halves and write the equivalence classes for each of the halves, get the pair `(2,2)` for the string `abab`, and the pair `(2,3)` for the string `abba`. Now compare these pairs, we get that `(2,2)<(2,3)`, which means that `abab < abba`.

#### Algorithm

Thus, we get the following algorithm. First, we form strings of one character and sort them by any sort algorithm in $O(nlogn)$. Next, logn times we make the transition from `k`
to `k+1`. Each transition is done like this: take sorted strings of length $2^k$, assign equivalence classes to them, then assign to each string of length $2^{k+1}$ a pair of numbers: equivalence classes of its halves, sort these pairs and get a sorted order for a string of length $2^{k+1}$.

The time complexity this algorithm will be $O(nlog2^n)$, because on at each of the $logn$
iterations we do the sorting in $O(nlogn)$ time.

```c++
int main() {
  string s;
  cin >> s;
  s += '$';
  int n;
  n = s.size();

  // p -> suffix array int, c -> equivalence class
  vector<int> p(n), c(n);

  // 1. k = 0:
  // sort characters with length 1
  // create array p that contains initial suffix array integers

  // single character
  vector<pair<char, int>> a(n); // (s[i], i)
  for (int i = 0; i < n; i++)
      a[i] = {s[i], i};

  sort(a.begin(), a.end());

  for (int i = 0; i < n; i++)
      p[i] = a[i].second;

  // calculate the equaivalenve class for each s[i..i+2^k-1]
  c[p[0]] = 0;
  for (int i = 1; i < n; i++)
  {
      if (a[i].first == a[i - 1].first)
      {
          c[p[i]] = c[p[i - 1]];
      }
      else
      {
          c[p[i]] = c[p[i - 1]] + 1;
      }
  }

  // strings with 2^k length
  int k = 0;
  while ((1 << k) < n)
  {
      // k -> k+1
      vector<pair<pair<int, int>, int>> x(n);
      for (int i = 0; i < n; i++)
      {
          x[i] = {{c[i], c[(i + (1 << k)) % n]}, i};
      }

      sort(x.begin(), x.end());

      for (int i = 0; i < n; i++)
          p[i] = x[i].second;

      c[p[0]] = 0;
      for (int i = 1; i < n; i++)
      {
          if (x[i].first == x[i - 1].first)
          {
              c[p[i]] = c[p[i - 1]];
          }
          else
          {
              c[p[i]] = c[p[i - 1]] + 1;
          }
      }

      k++;
  }

  for (int i = 0; i < n; i++)
  {
      cout << p[i] << " " << s.substr(p[i], n) << "\n";
  }

  return 0;
}
```

## 16. Heaps

Heap is a tree data structure that follows the following rules -

1. The `parent` should be `>=` or (`<=`) to it's `children` depending on whether it is a `max heap` or `min heap`.
2. The tree should form a complete binary tree i.e. any node is always be at a `h` or `h-1` level (where `h` is the height of the tree).

Following are some examples of trees that are heap and not a heap depending on the above rules.

![alt text](./Heaps/images/heaps-rules-example.png)

Based on the property of heaps, they can classified bw 2 types -

1. Min heap: The value of a node must be less than or equal to it's children's value.
2. Max heap: The value of a node must be greated than or equal to it's children's value.

Binary heaps are the ones where every parent has at most 2 children. In practice, binary heaps are enough to solve problems.

Binary heaps can be represented with an array of values where the children are followed after the parent.

As the heap is a complete binary tree array makes sense as there are no loss of space.

### Create a heap data structure

```cpp
#include <iostream>

using namespace std;

class Heap
{
private:
    int *array;
    int capacity;  // Max size of heap possible
    int count;     // Number of elements in heap
    int heap_type; // min or max heap

public:
    Heap *createHeap(int capacity, int heap_type)
    {
        Heap *h = (Heap *)malloc(sizeof(Heap));
        if (h == NULL)
        {
            cout << "Memory error" << "\n";
            return;
        }

        h->array = (int *)malloc(sizeof(int) * capacity);
        h->capacity = capacity;
        h->count = 0;
        h->heap_type = heap_type;

        return h;
    }
};
```

The class `Heap` contains the following variables -

1. `capacity`: Maximum size of the `Heap` data structure possible.
2. `array`: dynamically created array memory of size `sizeof(int)*h->capacity`
3. `count`: No of elements that are present in the heap right now
4. `heap_type`: Type of the heap - min heap(0) or max heap(1)

### Parent of a node

To find a parent of a node, we can use the formula -
$(i-1)/2$

Where i is the index of the child node.

Example: [7 5 6 1 4 3 2] is a max heap, and for the child node `6` which is at `index=2` the parent is present at `2-1/2 = 0`.

```cpp
int getParent(Heap *h, int i)
{
    if (i < 0 || i >= h->capacity)
        return -1;

    return (i - 1) / 2;
}
```

### Child of a node

To find the children of a node, we can use this formula -
$left=2*i+1, right=2*i+2$

Where i is the index of the child node.

Example: [7 5 6 1 4 3 2] is a max heap, and for the node `6` which is at `index=2` the left child is present at `2*2+1 = 5` and right child at `2*2+2=6`.

```cpp
int getLeftChild(Heap *h, int i)
{
    if (i < 0 || i >= h->capacity)
        return -1;

    return i * 2 + 1;
}

int getRightChild(Heap *h, int i)
{
    if (i < 0 || i >= h->capacity)
        return -1;

    return i * 2 + 2;
}
```

### Getting the maximum element

For a max heap, getting the maximum element is a $O(1)$ complexity task as we just have to return the root element.

```cpp
int getMax(Heap *h)
{
    if (h->count == 0)
        return -1;

    return h->array[0];
}
```

### Heapifying an element

After inserting an element, it may not satisfy the heap property. In that case, we need to adjust the heap positions again to follow the heap rule. This process if called `heapifying`. To heapify, we need to find the maximum element(for max heap) of a node and replace the current node with that maximum node.

We have to repeat this process for all nodes where the children are greater than the parent, finally we get a heap satisfying all rules.

It is observed that if a node is not satisfying the property of a heap, then from that node to the root all nodes have the same problem.

![alt text](max-heap-heapify.png)

```cpp
void heapify(Heap *h, int i)
{
    int left, right;
    left = getLeftChild(h, i);
    right = getRightChild(h, i);

    // Finds the maximum out of all 3 - parent and two children
    int max = i;
    if (left > -1 && h->array[max] < h->array[left])
    {
        max = left;
    }
    if (right > -1 && h->array[max] < h->array[right])
    {
        max = right;
    }

    if (max != i)
    {
        // Swap the parent with the max child node
        int temp = h->array[max];
        h->array[max] = h->array[i];
        h->array[i] = temp;

        // heapify the next layer
        heapify(h, max);
    }
}
```

Time complexity: $O(logn)$

## 17. Graphs

### Introduction to Graph

Tree is a non-linear data structure which represents hierarchy, it uses parent-child relationship to represent the hierarchy.

Now, consider a friendship relation, there are cycles between the friends, one friend is friend of another. In this case, we can't use tree to represent such structure. We cannot use trees when we have random connections among the nodes.

For this kind of relationship, we use graph data structure where random relations are allowed.

Graph is represented by a pair of sets: $G = (V, E)$.
$V$ is the set of vertices of the graph. $E$ is the set of edges in the graph.

Edges are represented with pairs of vertices $(v_i, v_j)$.

### Directed and Undirected Graph

There are 2 types of graphs - Directed and Undirected graphs.

Directed graphs has a certain order in the edges. $(v_i, v_j)$ means there is a directed edge from $v_i$ to $v_j$ and we can't go to $v_j$ from $v_i$ if no such edge is mentioned.

But, in undirected graph $(v_i, v_j)$ means we can go to $v_j$ from $v_i$ and to $v_i$ from $v_j$.

Examples: Social network follows undirected graph concept as one is a friend of another then it could be other way around as well. World wide web is an example of directed graph as one page leads to another page only.

### Degree of a vertex

In undirected graph, degree of a vertex is considered to be the number of edges passing through that vertex.

For a graph with set of edges: $E = (v_1, v_2), (v_1, v_3), (v_2, v_4), (v_3, v_4), (v_3, v_5), (v_4, v_5)$. Degrees of vertices $v_1, v_3$: $degree(v_1) = 2$, $degree(v_3) = 3$.
![Undirected Graph and Degree](./Graph/images/undirected_degree.png)

For directed graph, there are 2 types of degrees: InDegree, OutDegree.

- InDegree: Edges coming towards the vertex.
- OutDegree: Edges going out of the vertex.

![Directed Graph and InDegree and OutDegree](./Graph/images/directed_degree.png)

Some observations:

1. In directed graph, sum of indegrees is total no of edges and sum of outdegrees is also total no of edges. Every edge either contributes as indegree or outdegree for any of the vertices.
2. Maximum no of edges possible in a directed graph $V*(V-1)$(Not considering self-loops). Every vertex is connected to $V-1$ vertices maximum.
3. In undirected graph, sum of degrees is $2 * V$.
4. Maximum no of edges possible in a undirected graph $V*(V-1) / 2$(Not considering self-loops).

### Walk, Path, Cyclic and Acyclic

- **Walks**: Sequence of vertices that we can get by following the edges. From the diagram: $v_1, v_2, v_4, v_2$ is a walk. $v_1, v_5$ is not a walk as there is no edge b/w $v_1$ to $v_5$. In some cases it is mentioned as path.
- **Path**: Special kind of walk where no vertex is repeated. $v_1, v_2, v_4$ is a path. In some cases, it is mentioned as simple path.
- **Cyclic**: If there exists a walk that begins and ends with the same vertex.
  ![Unidirected and directed cyclic graph](./Graph/images/cyclic_graphs.png)
- **Acyclic**: There exists no such walk that ends and begins with the same vertex.

### Weighted graphs

If the edges of a graph has weights associated with them, then the graph is called weghted graph.

### Graph Representation

Graph can be represented in many ways but there 2 most popular ways that we will discuss here. They are - `adjacency matrix` and `adjacency list`.

**Adjacency Matrix**:

Adjacency matrix is a matrix with values 0/1. Matrix is `V x V` matrix where `V` is the number of vertices in the graph.

If there is an edge from $i$ to $j$ then $V_{ij} = 1$ otherwise it is 0.

If there are no self-loops then all diagonal elements in the matrix are 0.

For undirected graph, adjacency matrix is symmetric, as edge from $i$ to $j$ also means edge from $j$ to $i$.

![Adjacency matrix for undirected graph](./Graph/images/adjacency_matrix_undirected.png)

For directed graph, the matrix may not be symmetric.

_Special case when the vertex are not numbers but strings_

In that case, we need additional data structures. We can store the strings in an array and represent the strings with the index in that array.

In some cases, we may need reverse mapping as well. Like cities near "Kolkata", then we need hash as well to quickly get back the index. After getting the index, search for neighbouring cities from matrix on the same row, then use those indices to get the city names from the array.

_Time and Space Complexity_:

Space: $\theta(VXV)$

Operations:

1. Check if u and v are adjacent: $\theta(1)$. We just need to check $v_{ij}$ is 1/0.
2. Find all vertices adjacent to u: $\theta(V)$. We need to check the row of vertex V, which is a linear time operation.
3. Find degree of a vertex u: $\theta(V)$. Go through the row to check how many edges are connected.
4. Add/remove edges from a vertex u to v: $\theta(1)$. Just replace 1 by 0 for the cell `[u, v]`.
5. Add/remove vertex from a graph: $\theta(V*V)$. Make a matrix with 1 more/less row-column and then copy the vertices with new vertex edges/without deleted vertex edges.

**Adjacency List**:

There are certain issues with the adjacency matrix like redundant information. Adjacency matrix not only stores edges connected to it, but also edges not connected to it. Also it makes it difficult to traverse the adjacent vertices of a vertex, which is a very common operation in graph, as it takes linear time.

Adjacency list instead stores only the information that is needed for a edge. For every vertex it stores only the edges that are connected to it in a list.

Adjacency list representation is an array of lists, where lists are most popularly represented by - dynamic sized arrays, linked lists.

We are mainly using dynamic sized array because of their cache-friendlyness. Although there are some disadvantages like waste of memory incase the size is doubled due to addition of extra elements. Usually adding an item takes constant time, but due to doubling the size it may have to copy the array again.

![Adjacency list representation of directed graph](./Graph/images/adjacency_list_directed.png)

_Time and Space Complexity_:

Space: $O(V+E)$ for directed graph, $O(V+2*E)$ for undirected graph.

Operations:

1. Find if there is an edge b/w u to v: $O(V)$.
2. Find all adjacent of edge u: $\theta(degree(u))$.
3. Find degree of u: $\theta(1)$.
4. Add an edge: $\theta(1)$.
5. Remove an edge: $O(V)$.

### Adjacency List implementation in C++

To create an adjacency list like the following in C++,
![Adjacency list implementation C++](./Graph/images/adjacency_list_implementation.png)

You first have to create an array of vectors with size `V` which is the no of vertices in the graph.

```c++
#include <iostream>
#include <vector>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int main()
{
    int v = 4;
    vector<int> adj_list[v];
    addEdge(adj_list, 0, 1);
    addEdge(adj_list, 0, 2);
    addEdge(adj_list, 1, 2);
    addEdge(adj_list, 2, 3);
    return 0;
}
```

As we are implementing a undirected graph, we need to add `v` to `u` as well as `u` to `v` in the `add_edge` function.

Now print the graph using the following code,

```c++
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
```

Output:

```
0->1 2
1->0 2
2->0 1 3
3->2
```

### Breadth First Search

#### Introduction

V1: Given an undirected graph and a source vertex `S`, traverse the whole graph using BFS. In BFS, we have to make sure that every vertex is visited exactly once.

BFS is a sorted search in the sense that it prints all the immediate adjacent of the current vertex and then moves to the next immediate adjacents.

Example of social network can give clarity about it. You will print yourself(source) first, then you print your friends and then you print your friends' friends and so on.

Here the order in which you are printing your friends does not matter.

#### Examples

Following are some examples of BFS,

![BFS of an undirected graph](./Graph/images/BFS.png)

![BFS of another undirected graph](./Graph/images/BFS2.png)

#### Algorithm

The algorithm to do a BFS manages one queue `q` and an array `visited`. For every vertex we add it to the queue and mark this vertex as visited, then look for all adjacent vertices which are not visited and add them to the queue. The algorithm is described in detail here -

1. Initialize an array `visited` of length `V` with `false` indicating no vertex is visited yet. Initialize an empty queue `q` and push the source vertex `s` to it.
2. Get the front of the queue and print the vertex, remove the front from the queue.
3. Traverse all adjacent vertex to the recently printed vertex. Add any vertex to the queue which is not yet visited.
4. Repeat 2-3 till queue is not empty.

We will understand the algorithm using the following example:

![BFS algorithm example](./Graph/images/bfs_example.png)

1. `visited` array is initialized with all `false` values and queue `q` in empty.

   ![BFS example step 1](./Graph/images/bfs_example_s1.png)

2. Add source to the queue and set the `visited[s]` to `true`.

   ![BFS example step 2](./Graph/images/bfs_example_s2.png)

3. Take vertex `0` from the queue, print `0`, explore this vertex and mark vertex `1` and `2` which was not visited as visited.

   ![BFS example step 3](./Graph/images/bfs_example_s3.png)

4. Take vertex `1` from the queue, print `1`, explore this vertex and mark vertex `3` which was not visited as visited.

   ![BFS example step 4](./Graph/images/bfs_example_s4.png)

5. Take vertex `2` from the queue, print `2`, explore this vertex and mark vertex `4` which was not visited as visited.

   ![BFS example step 5](./Graph/images/bfs_example_s5.png)

6. Take vertex `3` from the queue, print `3`, no vertex adjacent to it is unvisited so do nothing.

   ![BFS example step 6](./Graph/images/bfs_example_s6.png)

7. Take vertex `4` from the queue, print `4`, no vertex adjacent to it is unvisited so do nothing.

   ![BFS example step 7](./Graph/images/bfs_example_s7.png)

#### Implementation

```c++
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void BFS(vector<int> adj[], int V, int s)
{
    bool visited[V] = {false};
    queue<int> q;

    q.push(s);
    visited[s] = true;

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

int main()
{
    int V;
    V = 5;
    vector<int> adj[V];
    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 3);
    addEdge(adj, 2, 4);
    addEdge(adj, 3, 4);

    BFS(adj, V, 0);

    return 0;
}
```

**Second Version**
In the first version, you just had to traverse the connected graph when the source is given. In this case, you are not given any source vertex and possibly the graph could be disconnected, how do you traverse the whole graph in BFS manner? Consider the following graph.

![No source and disconnected BFS](./Graph/images/bfsv2.png)

To traverse the above disconnected graph in BFS manner, we have to consider all vertex as possible source and check whether the vertex is visited or not. If the vertex is not visited then we call BFS considering this vertex as the source vertex.

```c++
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
```

Time complexity of this solution might seem bigger but if looked closely then it will come out to be $O(V+E)$. Why `V+E`? It is is because we are traversing all the adjacent edges of every vertex inside `BFS` function which is equal to `2*E` but why `V`? it is because in the worst case all the vertices are disconnected and we have to loop through all those vertices one by one in the `BFSDis` function.

**Question: No of components in a graph / No of islands in a graph**
Components in a graph are all the graphs which are connected.

To find out the no of components in a disconnected graph, we have to increment the count which is initially `0` whenever we see an unvisited vertex.

```c++
void BFSDis(vector<int> adj[], int V)
{
    int count=0;
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            BFS(adj, i, visited);
            count++;
        }
    }
    return count;
}
```

#### Applications

1. Shortest path in an undirected graph: BFS traverses the vertices according to their levels. The vertices which are 1-edge away from the source will be traversed first and then vertices that are 2-edge away.

2. Crawlers in search engine: In web, documents has link to other documents and they has links to some other documents. Web crawlers use BFS to crawl the web by going to the immediate next documents that has link in the current document and then the documents linked to those documents.

3. Peer to peer network: Sites like BitTorrent use BFS to search for immediate neighbours and then their neighbours and so on.

4. Social networking search

5. In garbage collection (Cheney's algorithm)

6. Cycle detection

7. Ford fulkerson algorithm

8. Broadcasting

### Depth First Search

Given a source vertex, visit all the other vertices in DFS manner.

If a source vertex is there then in DFS, we first print this and then we recursively call DFS for one of it's adjacent. After we are done visiting all of the connected vertices of it's adjacent and we do the same for other adjacents.

For DFS, we follow a recursive approach. For every unvisited vertex, we find it's adjacent which is not yet visited, then go to that unvisited vertex, do the same for this vertex and go through all the vertices in a depth wise manner.

#### Implementation

```cpp
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
```

#### Implementation for disconnected graph

```cpp
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
```

#### Implementation for counting connected components

```c++
int BFSDis(vector<int> adj[], int V)
{
    int count=0;
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            DFSRec(adj, i, visited);
            count++;
        }
    }
    return count;
}
```

#### Applications of DFS

1. Cycle Detection: DFS helps finding cycles in a graph. In DFS we naturally find after a certain depth whether the vertex has any visited vertex or not, if yes then it means it has cycle.
2. Topological Sorting: Topological sorting uses DFS and it has an application in file making in software engineering. When there are dependencies among different files, topological sorting helps in scheduling which file should be executed before others to avoid dependency issues.
3. Strongly connected components: Strongly connected components are solved using DFS. Mainly there are 2 algorithms kosaraju's algorithm and tarjan's algorithm.
4. Maze problems and puzzles: Most puzzle problems where we have an initial state and we search through each solution in the tree leaf node whether that is a solution or not, for that we always prefer DFS as it gives better performance in finding the solution.
5. Path finding: It is natural to use DFS when printing the path from source to destination as it always goes in the depth first manner and contains only the vertex that leads to the destination.

### Shortest Path for an undirected graph

Shortest path for a undirected graph is the path from source to destination vertex with least no of edges.

![alt text](./Graph/images/undirected-graph-shortest-path-ex1.png)

Shortest path: `[0 1 1 2]`

![alt text](./Graph/images/indirected-graph-shortest-path-ex2.png)

Shortes path: `[0 1 1 2 1 2]`

```cpp
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
```

Algorithm:

1. Initialize `dist[V]` with `{INF, INF, ...}`
2. `dist[source] = 0`
3. Create a queue `q`
4. Initialize the array `visited[V]` with `{false, false, ...}`
5. q.push(source), `visited[source] = true`
6. till q is not empty do the following -
   1. Get the front vertex `v` in the queue.
   2. Pop the first vertex `v` from queue.
   3. Traverse all non-visited vertices reachable from `v`
   4. Calculate the `dist[u]=dist[v]+1`, `visited[u]=true`, push `u` to `q`.
7. Traverse the `dist` to get the shortest distance from `source`.

Time complexity: $O(V+E)$

### Detect cycle in an undirected graph

Cyclic graph is when there exists one walk that begins and ends with the same vertex.

Detecting cycle in an undirected graph is easier than doing so in a directed graph.

We can use DFS to find cycles in an undirected graph by checking whether we can find any vertex whose adjacent is already visited or not.
But wait, just this condition will give you wrong answer. Because even when there are 2 vertices (0-1) it will say it has cycle, because just like 1 is an adjacent of 0, 0 is also an adjacent of 1.

So, we are keeping track of the parent vertex, and avoiding the case when the adjacent is the parent otherwise there is a cycle.

![alt text](./Graph/images/undirected-graph-cycle.png)

```cpp
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
```

In the code, we start with vertex 0, move from 1 -> 2 -> 3. Then when we see the `visited[3]=true` we return `true` for `u=1` as it is not the parent.

If there is no cycle in the graph, we return false at the end.

```cpp
 if (DFSRec(adj, visited, u, source))
    return true;
```

This code interrupts the loop and directly return true if any cycle is found for any adjacent vertex.

For cases where no such cycle is found, we move forward with other adjacent vertices to check the same.

Don't do the following, as it may return false without visiting other adjacent vertices which is wrong.

```cpp
return DFSRec(adj, visited, u, source)
```

Time Complexity: $O(V+E)$

### Detect cycle in a directed graph

### Topological Sort

Topological sorting is used when we need to schedule a series of tasks that needs to fulfill some pre-requisite.

For example, before starting task B, you need to complete task A then only you can move with task B.

This relationship can be represented with DAG or Directed Acyclic Graph. Directed as we need the relationship of which task come after which task, Acyclic as cycles will conflict the idea of prerequisites as one task cannot be completed without violating atleast one of the prerequisites.
