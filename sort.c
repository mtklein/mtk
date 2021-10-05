#include "sort.h"

static int* min(int *ptr, const int *end) {
    int *m = ptr;
    while (++ptr != end) {
        if (*m > *ptr) {
            m = ptr;
        }
    }
    return m;
}

void sort(int arr[], int n) {
    for (const int *end = arr+n; arr != end; arr++) {
        int *m = min(arr,end);

        int tmp = *arr;
        *arr = *m;
        *m = tmp;
    }
}

void sort_to(int out[], const int in[], int n) {
    for (int i = 0; i < n; i++) {
        out[i] = in[i];
    }
    sort(out,n);
}
