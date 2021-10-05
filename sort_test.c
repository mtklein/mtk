#include "checksum.h"
#include "len.h"
#include "sort.h"
#include "test.h"

static double random_numbers_in_place(int k, double *scale, const char* *unit) {
    int ref[1000];
    *scale = len(ref);
    *unit  = "ints";

    for (int i = 0; i < len(ref); i++) {
        ref[i] = (int)mix((uint32_t)i);
    }

    double elapsed = 0;
    while (k --> 0) {
        int arr[len(ref)];
        for (int i = 0; i < len(ref); i++) {
            arr[i] = ref[i];
        }

        elapsed -= now();
        sort(arr, len(arr));
        elapsed += now();
    }
    return elapsed;
}

static double random_numbers(int k, double *scale, const char* *unit) {
    int ref[1000];
    *scale = len(ref);
    *unit  = "ints";

    for (int i = 0; i < len(ref); i++) {
        ref[i] = (int)mix((uint32_t)i);
    }

    int arr[len(ref)];
    double start = now();
    while (k --> 0) {
        sort_to(arr, ref,len(ref));
    }
    return now() - start;
}

int main(int argc, char** argv) {
    {
        int arr[] = { 3, 2, 4, -5, 7, 1, 8 };
        sort(arr, len(arr));

        expect_eq(arr[0], -5);
        expect_eq(arr[1],  1);
        expect_eq(arr[2],  2);
        expect_eq(arr[3],  3);
        expect_eq(arr[4],  4);
        expect_eq(arr[5],  7);
        expect_eq(arr[6],  8);
    }

    {
        int ref[] = { 3, 2, 4, -5, 7, 1, 8 },
            arr[len(ref)] = {0};
        sort_to(arr, ref,len(ref));

        expect_eq(arr[0], -5);
        expect_eq(arr[1],  1);
        expect_eq(arr[2],  2);
        expect_eq(arr[3],  3);
        expect_eq(arr[4],  4);
        expect_eq(arr[5],  7);
        expect_eq(arr[6],  8);
    }

    bench(random_numbers_in_place);
    bench(random_numbers);

    return 0;
}
