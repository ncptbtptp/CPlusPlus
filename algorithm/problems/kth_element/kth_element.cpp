#include "kth_element.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

// Return pivot position(0-based)
// Randomize position and then move pivot to first position
// Both ends are inclusive: [left, right]
int partition(int *left, int *right)
{
    int pivotPos = rand() % (right - left + 1);
    std::swap(left[0], left[pivotPos]);

    int pivot = *left, *low = left + 1, *high = right;
    while (low <= high) {
        if (*low <= pivot) {
            *(low - 1) = *low;
            ++low;
        }
        else {
            std::swap(*low, *high);
            --high;
        }
    }
    *high = pivot;
    return (high - left);
}

void helper(int *left, int *right, int k, int &value)
{
    if (left == right) {
        value = *left;
        return;
    }

    // 0-based
    int pivotPos = partition(left, right);
    if (pivotPos == k - 1) {
        value = left[pivotPos];
    }
    else if (pivotPos < k - 1) {
        helper(left + pivotPos + 1, right, k - pivotPos - 1, value);
    }
    else {
        helper(left, left + pivotPos - 1, k, value);
    }
}

bool kth_element(vector<int> &a, int k, int &value)
{
    int n = a.size();
    if (k < 1 || k > n) {
        return false;
    }

    helper(a.data(), a.data() + n - 1, k, value);
    return true;
}
