#include <bits/stdc++.h>

// psudo code for merge
// void merge(int a[], int b[], int m, int n)
// {
//     i = 0, j = 0;
//     while (i < m && j < n)
//     {
//         if (a[i] <= b[j])
//         {
//             print a[i];
//             i++;
//         }
//         else
//         {
//             print b[j];
//             j++;
//         }
//     }

//     while (i < m)
//     {
//         print a[i];
//         i++;
//     }

//     while (j < n)
//     {
//         print b[j];
//         j++;
//     }
// }

void merge(int a[], int low, int mid, int high)
{
    int n1 = mid - low + 1, n2 = high - mid;
    int left[n1], right[n2];
    for (int i = 0; i < n1; i++)
        left[i] = a[low + i];
    for (int i = 0; i < n2; i++)
        right[i] = a[mid + i + 1];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            a[k] = left[i];
            i++;
            k++;
        }
        else
        {
            a[k] = right[j];
            j++;
            k++;
        }
    }

    while (i < n1)
    {
        a[k] = left[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        a[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(int a[], int l, int h)
{
    if (l < h) // atleast 2 elements
    {
        int m = l + (h - l) / 2;
        mergeSort(a, l, m);
        mergeSort(a, m + 1, h);
        merge(a, l, m, h);
    }
}

int main()
{
    int a[] = {20, 1, 30, 5, 8, 40, 31};
    int n = sizeof(a) / sizeof(a[0]);
    int l = 0, h = n - 1;
    mergeSort(a, l, h);
    return 0;
}