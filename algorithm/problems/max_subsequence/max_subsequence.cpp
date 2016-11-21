#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "max_subsequence.h"

using namespace std;

string get_max_subsequence(const string &str1, const string &str2)
{
    if (str1.empty() || str2.empty()) {
        return string();
    }

    // dp help us to reconstruct max subsequence
    enum Path {
        Left_up,
        Left,
        Up,
        Unknown
    };

    // First row/column served as sentinels
    int len1 = str1.length(), len2 = str2.length();
    vector<vector<int>> dp_res(len1 + 1, vector<int>(len2 + 1, 0));
    vector<vector<Path>> dp_path(len1 + 1, vector<Path>(len2 + 1, Unknown));

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            if (str1[i-1] == str2[j-1]) {
                dp_res[i][j] = dp_res[i-1][j-1] + 1;
                dp_path[i][j] = Left_up;
            }
            else if (dp_res[i-1][j] > dp_res[i][j-1]) {
                dp_res[i][j] = dp_res[i-1][j];
                dp_path[i][j] = Up;
            }
            else {
                dp_res[i][j] = dp_res[i][j-1];
                dp_path[i][j] = Left;
            }
        }
    }

    // No common characters
    if (dp_res[len1][len2] == 0) {
        return string();
    }

    string res(dp_res[len1][len2], '0');
    auto it = res.rbegin();
    size_t i = len1, j = len2;
    while (i > 0 && j > 0) {
        if (dp_path[i][j] == Left_up) {
            *(it++) = str1[i-1];
            --i;
            --j;
        }
        else if (dp_path[i][j] == Left) {
            --j;
        }
        else {
            --i;
        }
    }

    return res;
}
