#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "max_substring.h"

using namespace std;

string get_max_substring(const string &str1, const string &str2)
{
    if (str1.empty() || str2.empty()) {
        return string();
    }

    // First row/column served as sentinels
    vector<vector<int>> dp(str1.length() + 1, vector<int>(str2.length() + 1, 0));

    // Find all
    // row-by-row(i in str1), column-by-column(j in str2)
    for (size_t i = 0; i < str1.length(); ++i) {
        for (size_t j = 0; j < str2.length(); ++j) {
            if (str1[i] == str2[j]) {
                dp[i+1][j+1] = dp[i][j] + 1;
            }
            else {
                dp[i+1][j+1] = std::max(dp[i][j+1], dp[i+1][j]);
            }
        }
    }

    // max substring is the max of all possible common suffix strings
    int max_len = 0, max_i = 0;
    for (size_t i = 1; i <= str1.length(); ++i) {
        for (size_t j = 1; j <= str2.length(); ++j) {
            if (dp[i][j] > max_len) {
                max_len = dp[i][j];
                max_i = i-1;
            }
        }
    }

    return std::move(str1.substr(max_i - max_len + 1, max_len));
}
