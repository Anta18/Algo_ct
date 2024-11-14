#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Define maximum number of vertices
#define MAX 100

// Structure for adjacency list node
typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
} AdjListNode;

// Structure for adjacency list
typedef struct AdjList
{
    AdjListNode *head;
} AdjList;

// Structure for graph
typedef struct Graph
{
    int V;
    AdjList *array;
    int **adjMatrix;
} Graph;

// Structure for edge used in Kruskal's algorithm
typedef struct Edge
{
    int src;
    int dest;
    int weight;
} Edge;

// Structure for priority queue node used in Dijkstra's algorithm
typedef struct MinHeapNode
{
    int v;
    int dist;
} MinHeapNode;

// Structure for priority queue
typedef struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
} MinHeap;

// Structure for DSU (Disjoint Set Union)
typedef struct DSU
{
    int *parent;
    int *rank;
    int size;
} DSU;

// Function prototypes
Graph *createGraph(int V);
AdjListNode *newAdjListNode(int dest, int weight);
void addEdgeList(Graph *graph, int src, int dest, int weight, int directed);
void addEdgeMatrix(Graph *graph, int src, int dest, int weight, int directed);
void printAdjList(Graph *graph);
void printAdjMatrix(Graph *graph);
void BFS_List(Graph *graph, int start);
void DFS_ListUtil(Graph *graph, int v, int *visited);
void DFS_List(Graph *graph, int start);
Graph *reverseGraph(Graph *graph);
Graph *convertUndirectedToDirected(Graph *graph);
void topologicalSortDFSUtil(Graph *graph, int v, int *visited, int *stack, int *top);
void topologicalSortDFS(Graph *graph);
void topologicalSortBFS(Graph *graph);
void shortestPathBFS(Graph *graph, int start);
void shortestPathDijkstra(Graph *graph, int start);
void shortestPathBellmanFord(Graph *graph, int V, Edge edges[], int E, int start);
void primMST(Graph *graph);
void kruskalMST(Graph *graph, Edge edges[], int E);
int findSet(int parent[], int i);
void unionSet(int parent[], int rank[], int x, int y);
MinHeapNode *newMinHeapNode(int v, int dist);
MinHeap *createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b);
void minHeapify(MinHeap *minHeap, int idx);
int isEmpty(MinHeap *minHeap);
MinHeapNode *extractMin(MinHeap *minHeap);
void decreaseKey(MinHeap *minHeap, int v, int dist);

// Queue structure for BFS and Kahn's algorithm
typedef struct Queue
{
    int items[MAX];
    int front;
    int rear;
} Queue;

// Queue functions
void initQueue(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

int isEmptyQueue(Queue *q)
{
    return q->front == -1;
}

void enqueue(Queue *q, int value)
{
    if (q->rear == MAX - 1)
        return;
    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue *q)
{
    if (isEmptyQueue(q))
        return -1;
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear)
        initQueue(q);
    return item;
}

// DSU functions
DSU *createDSU(int size)
{
    DSU *dsu = (DSU *)malloc(sizeof(DSU));
    dsu->size = size;
    dsu->parent = (int *)malloc(size * sizeof(int));
    dsu->rank = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

int dsuFind(DSU *dsu, int x)
{
    if (dsu->parent[x] != x)
        dsu->parent[x] = dsuFind(dsu, dsu->parent[x]); // Path compression
    return dsu->parent[x];
}

void dsuUnion(DSU *dsu, int x, int y)
{
    int xroot = dsuFind(dsu, x);
    int yroot = dsuFind(dsu, y);
    if (xroot == yroot)
        return;
    // Union by rank
    if (dsu->rank[xroot] < dsu->rank[yroot])
    {
        dsu->parent[xroot] = yroot;
    }
    else if (dsu->rank[xroot] > dsu->rank[yroot])
    {
        dsu->parent[yroot] = xroot;
    }
    else
    {
        dsu->parent[yroot] = xroot;
        dsu->rank[xroot]++;
    }
}

void displayDSU(DSU *dsu)
{
    printf("DSU Parent Array:\n");
    for (int i = 0; i < dsu->size; i++)
        printf("Element %d: Parent %d\n", i, dsu->parent[i]);
    printf("DSU Rank Array:\n");
    for (int i = 0; i < dsu->size; i++)
        printf("Element %d: Rank %d\n", i, dsu->rank[i]);
}

// Create a new adjacency list node
AdjListNode *newAdjListNode(int dest, int weight)
{
    AdjListNode *newNode = malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Create a graph with V vertices
Graph *createGraph(int V)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->V = V;

    // Create adjacency list
    graph->array = malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; i++)
        graph->array[i].head = NULL;

    // Create adjacency matrix
    graph->adjMatrix = malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
    {
        graph->adjMatrix[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
            graph->adjMatrix[i][j] = 0;
    }

    return graph;
}

// Add edge to adjacency list
void addEdgeList(Graph *graph, int src, int dest, int weight, int directed)
{
    // Add edge from src to dest
    AdjListNode *newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    if (!directed)
    {
        // Add edge from dest to src
        newNode = newAdjListNode(src, weight);
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
}

// Add edge to adjacency matrix
void addEdgeMatrix(Graph *graph, int src, int dest, int weight, int directed)
{
    graph->adjMatrix[src][dest] = weight;
    if (!directed)
        graph->adjMatrix[dest][src] = weight;
}

// Print adjacency list
void printAdjList(Graph *graph)
{
    for (int v = 0; v < graph->V; v++)
    {
        AdjListNode *pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d(w=%d)", pCrawl->dest, pCrawl->weight);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Print adjacency matrix
void printAdjMatrix(Graph *graph)
{
    printf("\nAdjacency Matrix:\n   ");
    for (int i = 0; i < graph->V; i++)
        printf("%3d", i);
    printf("\n");
    for (int i = 0; i < graph->V; i++)
    {
        printf("%3d", i);
        for (int j = 0; j < graph->V; j++)
            printf("%3d", graph->adjMatrix[i][j]);
        printf("\n");
    }
}

// BFS using adjacency list
void BFS_List(Graph *graph, int start)
{
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++)
        visited[i] = 0;

    Queue q;
    initQueue(&q);
    visited[start] = 1;
    enqueue(&q, start);

    printf("BFS Traversal: ");

    while (!isEmptyQueue(&q))
    {
        int v = dequeue(&q);
        printf("%d ", v);

        AdjListNode *pCrawl = graph->array[v].head;
        while (pCrawl)
        {
            if (!visited[pCrawl->dest])
            {
                visited[pCrawl->dest] = 1;
                enqueue(&q, pCrawl->dest);
            }
            pCrawl = pCrawl->next;
        }
    }
    printf("\n");
}

// DFS utility function
void DFS_ListUtil(Graph *graph, int v, int *visited)
{
    visited[v] = 1;
    printf("%d ", v);

    AdjListNode *pCrawl = graph->array[v].head;
    while (pCrawl)
    {
        if (!visited[pCrawl->dest])
            DFS_ListUtil(graph, pCrawl->dest, visited);
        pCrawl = pCrawl->next;
    }
}

// DFS using adjacency list
void DFS_List(Graph *graph, int start)
{
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++)
        visited[i] = 0;

    printf("DFS Traversal: ");
    DFS_ListUtil(graph, start, visited);
    printf("\n");
}

// Reverse the graph edges
Graph *reverseGraph(Graph *graph)
{
    Graph *rev = createGraph(graph->V);
    for (int v = 0; v < graph->V; v++)
    {
        AdjListNode *pCrawl = graph->array[v].head;
        while (pCrawl)
        {
            addEdgeList(rev, pCrawl->dest, v, pCrawl->weight, 1);
            pCrawl = pCrawl->next;
        }
    }
    return rev;
}

// Convert undirected graph to directed by keeping one direction
Graph *convertUndirectedToDirected(Graph *graph)
{
    Graph *dir = createGraph(graph->V);
    for (int v = 0; v < graph->V; v++)
    {
        AdjListNode *pCrawl = graph->array[v].head;
        while (pCrawl)
        {
            if (v < pCrawl->dest) // Keep edge from lower to higher
                addEdgeList(dir, v, pCrawl->dest, pCrawl->weight, 1);
            pCrawl = pCrawl->next;
        }
    }
    return dir;
}

// Topological Sort using DFS
void topologicalSortDFSUtil(Graph *graph, int v, int *visited, int *stack, int *top)
{
    visited[v] = 1;
    AdjListNode *pCrawl = graph->array[v].head;
    while (pCrawl)
    {
        if (!visited[pCrawl->dest])
            topologicalSortDFSUtil(graph, pCrawl->dest, visited, stack, top);
        pCrawl = pCrawl->next;
    }
    stack[(*top)++] = v;
}

void topologicalSortDFS(Graph *graph)
{
    int *stack = malloc(graph->V * sizeof(int));
    int topIdx = 0;
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++)
        visited[i] = 0;

    for (int i = 0; i < graph->V; i++)
    {
        if (!visited[i])
            topologicalSortDFSUtil(graph, i, visited, stack, &topIdx);
    }

    printf("Topological Sort (DFS): ");
    for (int i = topIdx - 1; i >= 0; i--)
        printf("%d ", stack[i]);
    printf("\n");
    free(stack);
}

// Topological Sort using BFS (Kahn's Algorithm)
void topologicalSortBFS(Graph *graph)
{
    int in_degree[graph->V];
    for (int i = 0; i < graph->V; i++)
        in_degree[i] = 0;

    // Compute in-degree
    for (int u = 0; u < graph->V; u++)
    {
        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl)
        {
            in_degree[pCrawl->dest]++;
            pCrawl = pCrawl->next;
        }
    }

    Queue q;
    initQueue(&q);
    for (int i = 0; i < graph->V; i++)
        if (in_degree[i] == 0)
            enqueue(&q, i);

    int cnt = 0;
    int topOrder[graph->V];

    while (!isEmptyQueue(&q))
    {
        int u = dequeue(&q);
        topOrder[cnt++] = u;

        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl)
        {
            if (--in_degree[pCrawl->dest] == 0)
                enqueue(&q, pCrawl->dest);
            pCrawl = pCrawl->next;
        }
    }

    if (cnt != graph->V)
    {
        printf("Graph has a cycle. Topological sort not possible.\n");
        return;
    }

    printf("Topological Sort (BFS/Kahn's): ");
    for (int i = 0; i < cnt; i++)
        printf("%d ", topOrder[i]);
    printf("\n");
}

// Shortest Path BFS (for unweighted graphs)
void shortestPathBFS(Graph *graph, int start)
{
    int distance[graph->V];
    for (int i = 0; i < graph->V; i++)
        distance[i] = -1;

    Queue q;
    initQueue(&q);
    distance[start] = 0;
    enqueue(&q, start);

    while (!isEmptyQueue(&q))
    {
        int u = dequeue(&q);
        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl)
        {
            if (distance[pCrawl->dest] == -1)
            {
                distance[pCrawl->dest] = distance[u] + 1;
                enqueue(&q, pCrawl->dest);
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("Shortest distances from vertex %d (BFS):\n", start);
    for (int i = 0; i < graph->V; i++)
        printf("Vertex %d: %d\n", i, distance[i]);
}

// Create a new MinHeap node
MinHeapNode *newMinHeapNode(int v, int dist)
{
    MinHeapNode *minHeapNode = malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Create a MinHeap
MinHeap *createMinHeap(int capacity)
{
    MinHeap *minHeap = malloc(sizeof(MinHeap));
    minHeap->pos = malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = malloc(capacity * sizeof(MinHeapNode *));
    return minHeap;
}

// Swap two MinHeapNodes
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b)
{
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

// Heapify at given index
void minHeapify(MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;
    if (smallest != idx)
    {
        // Swap positions
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// Check if MinHeap is empty
int isEmpty(MinHeap *minHeap)
{
    return minHeap->size == 0;
}

// Extract minimum node from MinHeap
MinHeapNode *extractMin(MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    MinHeapNode *root = minHeap->array[0];
    MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    minHeap->size--;
    minHeapify(minHeap, 0);

    return root;
}

// Decrease key value
void decreaseKey(MinHeap *minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

// Dijkstra's algorithm
void shortestPathDijkstra(Graph *graph, int src)
{
    int V = graph->V;
    int dist[V];
    for (int v = 0; v < V; v++)
        dist[v] = INT_MAX;
    dist[src] = 0;

    MinHeap *minHeap = createMinHeap(V);
    for (int v = 0; v < V; v++)
    {
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    minHeap->size = V;

    decreaseKey(minHeap, src, dist[src]);

    while (!isEmpty(minHeap))
    {
        MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl)
        {
            int v = pCrawl->dest;
            if (minHeap->pos[v] < minHeap->size && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("Dijkstra's shortest paths from vertex %d:\n", src);
    for (int i = 0; i < V; i++)
        printf("Vertex %d: %d\n", i, dist[i]);
}

// Bellman-Ford algorithm
void shortestPathBellmanFord(Graph *graph, int V, Edge edges[], int E, int src)
{
    int dist[V];
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    for (int i = 1; i < V; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }

    // Check for negative-weight cycles
    for (int j = 0; j < E; j++)
    {
        int u = edges[j].src;
        int v = edges[j].dest;
        int weight = edges[j].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
        {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Bellman-Ford shortest paths from vertex %d:\n", src);
    for (int i = 0; i < V; i++)
        printf("Vertex %d: %d\n", i, dist[i]);
}

// Prim's algorithm
void primMST(Graph *graph)
{
    int V = graph->V;
    int parent[V];
    int key[V];
    int inMST[V];
    for (int i = 0; i < V; i++)
    {
        key[i] = INT_MAX;
        inMST[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++)
    {
        // Find the minimum key vertex not yet included in MST
        int min = INT_MAX, u;
        for (int v = 0; v < V; v++)
            if (!inMST[v] && key[v] < min)
            {
                min = key[v];
                u = v;
            }

        inMST[u] = 1;

        // Update key and parent index of adjacent vertices
        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl)
        {
            int v = pCrawl->dest;
            if (!inMST[v] && pCrawl->weight < key[v])
            {
                key[v] = pCrawl->weight;
                parent[v] = u;
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("Prim's MST:\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d (w=%d)\n", parent[i], i, graph->adjMatrix[parent[i]][i]);
}

// Kruskal's algorithm
void kruskalMST(Graph *graph, Edge edges[], int E)
{
    int V = graph->V;
    // Sort edges based on weight using Bubble Sort (for simplicity)
    for (int i = 0; i < E - 1; i++)
    {
        for (int j = 0; j < E - i - 1; j++)
        {
            if (edges[j].weight > edges[j + 1].weight)
            {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // Initialize DSU
    DSU *dsu = createDSU(V);

    Edge result[V];
    int e = 0;
    int i = 0;
    while (e < V - 1 && i < E)
    {
        Edge next_edge = edges[i++];
        int x = dsuFind(dsu, next_edge.src);
        int y = dsuFind(dsu, next_edge.dest);

        if (x != y)
        {
            result[e++] = next_edge;
            dsuUnion(dsu, x, y);
        }
    }

    printf("Kruskal's MST:\n");
    for (int i = 0; i < e; i++)
        printf("%d - %d (w=%d)\n", result[i].src, result[i].dest, result[i].weight);

    // Free DSU memory
    free(dsu->parent);
    free(dsu->rank);
    free(dsu);
}

// Main function with a menu to demonstrate functionalities
int main()
{
    int V, E, directed;
    printf("Enter number of vertices: ");
    scanf("%d", &V);
    printf("Enter number of edges: ");
    scanf("%d", &E);
    printf("Is the graph directed? (1 for Yes, 0 for No): ");
    scanf("%d", &directed);

    Graph *graph = createGraph(V);
    Edge edges[E];
    printf("Enter edges in format (src dest weight):\n");
    for (int i = 0; i < E; i++)
    {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        addEdgeList(graph, src, dest, weight, directed);
        addEdgeMatrix(graph, src, dest, weight, directed);
        edges[i].src = src;
        edges[i].dest = dest;
        edges[i].weight = weight;
    }

    // Initialize DSU for independent usage
    DSU *dsu = createDSU(V);

    int choice, start;
    while (1)
    {
        printf("\nGraph Operations Menu:\n");
        printf("1. Print Adjacency List\n");
        printf("2. Print Adjacency Matrix\n");
        printf("3. BFS Traversal\n");
        printf("4. DFS Traversal\n");
        printf("5. Reverse Graph\n");
        printf("6. Convert Undirected to Directed\n");
        printf("7. Topological Sort (DFS)\n");
        printf("8. Topological Sort (BFS/Kahn's)\n");
        printf("9. Shortest Path BFS (Unweighted)\n");
        printf("10. Shortest Path Dijkstra\n");
        printf("11. Shortest Path Bellman-Ford\n");
        printf("12. Prim's MST\n");
        printf("13. Kruskal's MST\n");
        printf("14. DSU Operations Menu\n");
        printf("15. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printAdjList(graph);
            break;
        case 2:
            printAdjMatrix(graph);
            break;
        case 3:
            printf("Enter starting vertex for BFS: ");
            scanf("%d", &start);
            BFS_List(graph, start);
            break;
        case 4:
            printf("Enter starting vertex for DFS: ");
            scanf("%d", &start);
            DFS_List(graph, start);
            break;
        case 5:
        {
            Graph *rev = reverseGraph(graph);
            printf("Reversed Graph Adjacency List:\n");
            printAdjList(rev);
            // Free reversed graph if necessary
            // For simplicity, not freeing memory here
            break;
        }
        case 6:
        {
            if (directed)
            {
                printf("Graph is already directed.\n");
                break;
            }
            Graph *dir = convertUndirectedToDirected(graph);
            printf("Directed Graph Adjacency List:\n");
            printAdjList(dir);
            // Free directed graph if necessary
            // For simplicity, not freeing memory here
            break;
        }
        case 7:
            topologicalSortDFS(graph);
            break;
        case 8:
            topologicalSortBFS(graph);
            break;
        case 9:
            printf("Enter starting vertex for BFS Shortest Path: ");
            scanf("%d", &start);
            shortestPathBFS(graph, start);
            break;
        case 10:
            printf("Enter starting vertex for Dijkstra's algorithm: ");
            scanf("%d", &start);
            shortestPathDijkstra(graph, start);
            break;
        case 11:
            printf("Enter starting vertex for Bellman-Ford algorithm: ");
            scanf("%d", &start);
            shortestPathBellmanFord(graph, V, edges, E, start);
            break;
        case 12:
            if (directed)
            {
                printf("Prim's MST is not applicable for directed graphs.\n");
            }
            else
            {
                primMST(graph);
            }
            break;
        case 13:
            kruskalMST(graph, edges, E);
            break;
        case 14:
        {
            // DSU Operations Menu
            int dsu_choice, x, y;
            while (1)
            {
                printf("\nDSU Operations Menu:\n");
                printf("1. Find Set of an Element\n");
                printf("2. Union Sets of Two Elements\n");
                printf("3. Display DSU State\n");
                printf("4. Return to Main Menu\n");
                printf("Enter your DSU choice: ");
                scanf("%d", &dsu_choice);

                switch (dsu_choice)
                {
                case 1:
                    printf("Enter element to find its set: ");
                    scanf("%d", &x);
                    if (x < 0 || x >= V)
                    {
                        printf("Invalid element!\n");
                        break;
                    }
                    printf("Set of element %d: %d\n", x, dsuFind(dsu, x));
                    break;
                case 2:
                    printf("Enter two elements to union: ");
                    scanf("%d %d", &x, &y);
                    if (x < 0 || x >= V || y < 0 || y >= V)
                    {
                        printf("Invalid elements!\n");
                        break;
                    }
                    dsuUnion(dsu, x, y);
                    printf("Elements %d and %d have been united.\n", x, y);
                    break;
                case 3:
                    displayDSU(dsu);
                    break;
                case 4:
                    goto end_dsu_menu;
                default:
                    printf("Invalid DSU choice! Please try again.\n");
                }
            }
        end_dsu_menu:;
            break;
        }
        case 15:
            printf("Exiting...\n");
            // Free allocated memory before exiting
            // For simplicity, not freeing all memory here
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
