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

    int getParent(Heap *h, int i)
    {
        if (i < 0 || i >= h->capacity)
            return -1;

        return (i - 1) / 2;
    }

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

    int getMax(Heap *h)
    {
        if (h->count == 0)
            return -1;

        return h->array[0];
    }

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
};
