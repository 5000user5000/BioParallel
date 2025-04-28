#include "align_sw.hpp"
#include <vector>
#include <algorithm>
#include <sstream>

AlignmentResult smith_waterman(const std::string& seq1, const std::string& seq2,
                               int match, int mismatch, int gap) {
    size_t m = seq1.size(), n = seq2.size();
    std::vector<std::vector<int>> H(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> traceback(m + 1, std::vector<int>(n + 1, 0));

    int max_score = 0, max_i = 0, max_j = 0;

    // Fill DP table
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            int score_diag = H[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match : mismatch);
            int score_up   = H[i - 1][j] + gap;
            int score_left = H[i][j - 1] + gap;
            H[i][j] = std::max({0, score_diag, score_up, score_left});

            if (H[i][j] == score_diag) traceback[i][j] = 1;
            else if (H[i][j] == score_up) traceback[i][j] = 2;
            else if (H[i][j] == score_left) traceback[i][j] = 3;

            if (H[i][j] > max_score) {
                max_score = H[i][j];
                max_i = i;
                max_j = j;
            }
        }
    }

    // Traceback
    std::string align1, align2, match_line;
    int i = max_i, j = max_j;
    int end1 = i, end2 = j;

    while (i > 0 && j > 0 && H[i][j] > 0) {
        if (traceback[i][j] == 1) {
            align1 = seq1[i - 1] + align1;
            align2 = seq2[j - 1] + align2;
            match_line = (seq1[i - 1] == seq2[j - 1] ? "|" : "*") + match_line;
            --i; --j;
        } else if (traceback[i][j] == 2) {
            align1 = seq1[i - 1] + align1;
            align2 = "-" + align2;
            match_line = " " + match_line;
            --i;
        } else {
            align1 = "-" + align1;
            align2 = seq2[j - 1] + align2;
            match_line = " " + match_line;
            --j;
        }
    }

    return AlignmentResult {
        .score = max_score,
        .start1 = i, .end1 = end1 - 1,
        .start2 = j, .end2 = end2 - 1,
        .aligned_seq1 = align1,
        .aligned_seq2 = align2,
        .match_line = match_line
    };
}
