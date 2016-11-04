/* Given an integer array, rearrange numbers so that even numbers are listed before odd numbers.
 * Order inside the group is not important.
 * Must be O(n).
 */
#include <stdio.h>

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void group_even_odd(int A[], int size)
{
    if (size < 2) {
        return;
    }

    int even = 0, odd = size - 1;
    while (even <= odd) {
        if (A[even] % 2 == 0) {
            ++even;
        }
        else {
            swap(&A[even], &A[odd--]);
        }
    }
}

int main()
{
    int A[] = {1,2,3,3,4,5,6,6};
    group_even_odd(A, sizeof(A)/sizeof(A[0]));
    for (int i = 0; i < sizeof(A)/sizeof(A[0]); ++i) {
        printf ("%d ", A[i]);
    }
    printf("\n");
    return 0;
}
