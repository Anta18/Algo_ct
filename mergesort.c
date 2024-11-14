#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int l, int r)
{
    int *t = (int *)malloc((r - l + 1) * sizeof(int));
    int mid = (l + r) / 2;
    int i = l, j = mid + 1, k = 0;
    while (k < r - l + 1 && i <= mid && j <= r)
    {
        if (arr[i] <= arr[j])
            t[k++] = arr[i++];
        else
            t[k++] = arr[j++];
    }
    while (i <= mid)
    {
        t[k++] = arr[i++];
    }
    while (j <= r)
    {
        t[k++] = arr[j++];
    }
    i = l;
    for (int j = 0; j <= r - l; j++)
    {
        arr[i++] = t[j];
    }
    free(t);
}

void mergesort(int *arr, int l, int r)
{
    if (l >= r)
        return;
    int mid = (l + r) / 2;
    mergesort(arr, l, mid);
    mergesort(arr, mid + 1, r);
    merge(arr, l, r);
}
int main()
{
    int n;
    scanf("%d", &n);
    int *l = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &l[i]);
    }
    mergesort(l, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", l[i]);
    }

    return 0;
}