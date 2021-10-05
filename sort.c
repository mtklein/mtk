#include "sort.h"

void sort(int arr[], int n) {
    for (int *i = arr; i < arr+n; i++)
    for (int *j = i+1; j < arr+n; j++) {
        if (*i > *j) {
            int tmp = *i;
            *i = *j;
            *j = tmp;
        }
    }
}

void sort_into(int arr[], int n, int val) {
    (void)arr;
    (void)n;
    (void)val;
}
