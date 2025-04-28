#pragma once
#include <string>
#include "align_sw.hpp"  // Reuse AlignmentResult struct

AlignmentResult smith_waterman_cuda(
    const std::string& seq1,
    const std::string& seq2,
    int match = 2,
    int mismatch = -1,
    int gap = -2
);
