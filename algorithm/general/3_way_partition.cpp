// You have balls of three colors(1,2,3). Group balls of the same color, and
// sort them so that 1s come before 2s, which come before 3s.
// See: https://en.wikipedia.org/wiki/Dutch_national_flag_problem
//
// Time:  O(N)
// Space: O(1)
//
// TODO: gtest

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void partition(vector<int>& a)
{
    int n = a.size();
    if (n < 2) {
        return;
    }

    int i = 0, first = 0, third = n - 1;
    while (i <= third) {
        if (a[i] == 2) {
            ++i;
        }
        else if (a[i] == 1) {
            std::swap(a[i++], a[first++]);
        }
        else {
            std::swap(a[i], a[third--]);
        }
    }
}

int main()
{
    vector<int> balls = {1, 2, 3, 3, 2, 1};
    partition(balls);
    for_each(balls.cbegin(), balls.cend(), [](int i) { cout << i << " "; });
    cout << endl;
}
