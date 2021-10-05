#include "len.h"
#include "sort.h"
#include "test.h"

int main(void) {

    int arr[] = { 3, 2, 4, -5, 7, 1, 8 };
    sort(arr, len(arr));

    expect_eq(arr[0], -5);
    expect_eq(arr[1],  1);
    expect_eq(arr[2],  2);
    expect_eq(arr[3],  3);
    expect_eq(arr[4],  4);
    expect_eq(arr[5],  7);
    expect_eq(arr[6],  8);

    return 0;
}
