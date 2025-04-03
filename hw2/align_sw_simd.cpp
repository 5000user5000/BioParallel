#include "align_sw_simd.hpp"
#include <xsimd/xsimd.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

AlignmentResult smith_waterman_simd(const std::string& seq1, const std::string& seq2,
                                    int match, int mismatch, int gap) {
    using namespace xsimd;
    size_t m = seq1.size(), n = seq2.size();
    const std::size_t vec_size = batch<int>::size;

    // 矩陣行（橫向）一維化儲存
    std::vector<int> H((m + 1) * (n + 1), 0);

    auto idx = [&](int i, int j) { return i * (n + 1) + j; };

    int max_score = 0;
    int max_i = 0, max_j = 0;

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; j += vec_size) {
            batch<int> score_diag, score_up, score_left;
            batch<int> current;

            // 加載前一列的值
            batch<int> prev_row = batch<int>::load_unaligned(&H[idx(i - 1, j)]);
            batch<int> prev_diag = batch<int>::load_unaligned(&H[idx(i - 1, j - 1)]);
            batch<int> left = batch<int>::load_unaligned(&H[idx(i, j - 1)]);

            // 建立分數比較
            std::array<int, vec_size> match_arr;
            for (std::size_t k = 0; k < vec_size; ++k) {
                if (j + k <= n) {
                    match_arr[k] = (seq1[i - 1] == seq2[j + k - 1]) ? match : mismatch;
                } else {
                    match_arr[k] = 0;
                }
            }

            batch<int> match_score = batch<int>::load_unaligned(match_arr.data());

            score_diag = prev_diag + match_score;
            score_up = prev_row + batch<int>(gap);
            score_left = left + batch<int>(gap);

            current = max(score_diag, score_up);
            current = max(current, score_left);
            current = max(current, batch<int>(0));

            current.store_unaligned(&H[idx(i, j)]);

            for (std::size_t k = 0; k < vec_size; ++k) {
                if (j + k <= n && H[idx(i, j + k)] > max_score) {
                    max_score = H[idx(i, j + k)];
                    max_i = i;
                    max_j = j + k;
                }
            }
        }
    }

    // 🟡 只使用 scalar traceback
    return smith_waterman(seq1, seq2, match, mismatch, gap);
}
