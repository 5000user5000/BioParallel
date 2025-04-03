#pragma once
#include <string>

struct AlignmentResult {
    int score;
    int start1, end1;
    int start2, end2;
    std::string aligned_seq1;
    std::string aligned_seq2;
    std::string match_line;
};

AlignmentResult smith_waterman(
    const std::string& seq1,
    const std::string& seq2,
    int match = 2,
    int mismatch = -1,
    int gap = -2
);
