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

## Sorting

In this section, I will discuss about the sorting techniques. Each technique is used in certain situations and gives good result when used. Some of them has higher time complexity some of them has lower.

But, there are 2 factors that you will hear frequently when going through the sorting algorithms - Inplace, Stable.

- Inplace - sorting is performed on the original array or new sorted array is returned.
- Stable - Same values are ordered in the exact previous order or not.

## Insertion Sort

Insertion sort is a stable sort and an inplace algorithm that changes the original array without returning any sorted array.

The best way to describe this algorithm is by card shifting. When you have cards in your hand and you have some way to know which comes before the other one. You go through the cards from left to right. When you encounter a card that is smaller than the previous card, you start shifting the bigger cards to the right of this smaller card untill you find a card that is same or less than the smaller card on it's left side.

### Merge Sort

## Graphs

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
