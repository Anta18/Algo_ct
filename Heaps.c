#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CAPACITY 100

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int Root()
{
    return 1;
}

int Parent(int n)
{
    return n / 2;
}

int LeftChild(int n)
{
    return 2 * n;
}

int rightChild(int n)
{
    return 2 * n + 1;
}

bool hasParent(int n)
{
    return n != 1;
}

int getMin(int n, int H[])
{
    return H[Root()];
}

int shiftUp(int H[], int n)
{
    while (hasParent(n) && H[n] < H[Parent(n)])
    {
        swap(&H[n], &H[Parent(n)]);
        n = Parent(n);
    }
}

int shiftDown(int H[], int n, int size)
{
    while (LeftChild(n) <= size)
    {
        int child = LeftChild(n);
        if (rightChild(n) <= size && H[rightChild(n)] < H[LeftChild(n)])
        {
            child = rightChild(n);
        }
        if (H[n] > H[child])
            swap(&H[n], &H[child]);
        else
            break;
        n = child;
    }
}

void push(int H[], int newNum, int *size)
{
    if (*size == CAPACITY)
    {
        printf("Priority queue full! \n");
        exit(0);
    }
    H[++(*size)] = newNum;
    shiftUp(H, *size);
}

void pop(int H[], int *size)
{
    if (*size == 0)
    {
        printf("Priority Empty! \n");
        exit(0);
    }
    H[Root()] = H[*size];
    (*size)--;
    shiftDown(H, Root(), *size);
}

void buildHeap(int arr[], int H[], int *size, int n)
{
    for (int i = 0; i < n; i++)
    {
        H[i + 1] = arr[i];
    }
    *size = n;
    for (int i = n / 2; i >= 1; i--)
    {
        shiftDown(H, i, *size);
    }
}

void printHeap(int H[], int size)
{
    if (size == 0)
    {
        printf("Heap is empty.\n");
        return;
    }
    printf("Heap elements: ");
    for (int i = 1; i <= size; i++)
    {
        printf("%d ", H[i]);
    }
    printf("\n");
}

int main()
{
    int H[CAPACITY + 1]; // Heap array (1-based indexing)
    int size = 0;        // Current size of the heap

    // Example elements to insert into the heap
    int elements[] = {5, 3, 8, 1, 2, 9, 4};
    int n = sizeof(elements) / sizeof(elements[0]);

    printf("Building heap from array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", elements[i]);
    }
    printf("\n");

    // Build the heap using the buildHeap function
    buildHeap(elements, H, &size, n);
    printHeap(H, size);

    // Test getMin
    printf("Minimum element in the heap: %d\n", getMin(size, H));

    // Insert additional elements
    int newElements[] = {0, 7, 6};
    int m = sizeof(newElements) / sizeof(newElements[0]);

    for (int i = 0; i < m; i++)
    {
        printf("Inserting %d into the heap.\n", newElements[i]);
        push(H, newElements[i], &size);
        printHeap(H, size);
    }

    // Get the new minimum
    printf("New minimum element in the heap: %d\n", getMin(size, H));

    // Remove elements from the heap
    printf("Removing elements from the heap:\n");
    while (size > 0)
    {
        printf("Current min: %d\n", getMin(size, H));
        pop(H, &size);
        printHeap(H, size);
    }

    return 0;
}
