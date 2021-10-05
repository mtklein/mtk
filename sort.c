#include "sort.h"

void sort(int arr[], int n) {
    for (int *i = arr; i < arr+n; i++)
    for (int *j = arr; j < arr+n; j++) {
        if (*i < *j) {
            int tmp = *i;
            *i = *j;
            *j = tmp;
        }
    }
}
