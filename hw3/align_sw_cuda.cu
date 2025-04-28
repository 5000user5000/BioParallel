#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

#include "align_sw.hpp"  // AlignmentResult struct

__global__
void smith_waterman_kernel_wavefront(
    const char* seq1, const char* seq2,
    int* H,
    int m, int n,
    int match, int mismatch, int gap,
    int wave) // 現在要算第 wave 條斜線
{
    int tx = threadIdx.x;
    int ty = wave - tx;

    if (tx >= 1 && tx <= n && ty >= 1 && ty <= m) {
        int idx = ty * (n + 1) + tx;
        int idx_diag = (ty-1) * (n + 1) + (tx-1);
        int idx_up   = (ty-1) * (n + 1) + tx;
        int idx_left = ty * (n + 1) + (tx-1);

        int score_match = (seq1[ty-1] == seq2[tx-1]) ? match : mismatch;

        int score_diag = H[idx_diag] + score_match;
        int score_up = H[idx_up] + gap;
        int score_left = H[idx_left] + gap;

        int max_score = max(0, max(score_diag, max(score_up, score_left)));
        H[idx] = max_score;
    }
}


AlignmentResult smith_waterman_cuda(
    const std::string& seq1,
    const std::string& seq2,
    int match,
    int mismatch,
    int gap)
{
    size_t m = seq1.size(), n = seq2.size();
    std::vector<int> H((m+1)*(n+1), 0);

    // Allocate device memory
    char *d_seq1, *d_seq2;
    int *d_H;
    cudaMalloc(&d_seq1, m * sizeof(char));
    cudaMalloc(&d_seq2, n * sizeof(char));
    cudaMalloc(&d_H, (m + 1) * (n + 1) * sizeof(int));

    // Copy input to device
    cudaMemcpy(d_seq1, seq1.data(), m * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seq2, seq2.data(), n * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_H, H.data(), (m + 1) * (n + 1) * sizeof(int), cudaMemcpyHostToDevice);

    // Launch kernel
    dim3 blockSize(256);
    dim3 gridSize((n + blockSize.x - 1) / blockSize.x, m);

    // smith_waterman_kernel<<<gridSize, blockSize>>>(d_seq1, d_seq2, d_H, m, n, match, mismatch, gap);
    // cudaDeviceSynchronize();

    // Wavefront computation
    for (int wave = 2; wave <= m + n; ++wave) { // wave從2開始，因為座標都是從1開始
        int num_threads = min(wave-1, (int)min((size_t)n, (size_t)m));
    
        if (num_threads <= 0) continue; // 防止空 kernel
    
        smith_waterman_kernel_wavefront<<<1, wave>>>(d_seq1, d_seq2, d_H, m, n, match, mismatch, gap, wave);
        cudaDeviceSynchronize();
    }


    // Copy back results
    cudaMemcpy(H.data(), d_H, (m + 1) * (n + 1) * sizeof(int), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_seq1);
    cudaFree(d_seq2);
    cudaFree(d_H);

    // --- Find max score and position
    int max_score = 0;
    int max_i = 0, max_j = 0;
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (H[i*(n+1) + j] > max_score) {
                max_score = H[i*(n+1) + j];
                max_i = i;
                max_j = j;
            }
        }
    }

    // --- Traceback
    std::string aligned_seq1 = "";
    std::string aligned_seq2 = "";

    int i = max_i;
    int j = max_j;

    while (i > 0 && j > 0) {
        int idx = i*(n+1) + j;
        int idx_diag = (i-1)*(n+1) + (j-1);
        int idx_up = (i-1)*(n+1) + j;
        int idx_left = i*(n+1) + (j-1);

        if (H[idx] == 0)
            break;

        if (H[idx] == H[idx_diag] + ((seq1[i-1] == seq2[j-1]) ? match : mismatch)) {
            aligned_seq1 = seq1[i-1] + aligned_seq1;
            aligned_seq2 = seq2[j-1] + aligned_seq2;
            i--;
            j--;
        } else if (H[idx] == H[idx_left] + gap) {
            aligned_seq1 = '-' + aligned_seq1;
            aligned_seq2 = seq2[j-1] + aligned_seq2;
            j--;
        } else {
            aligned_seq1 = seq1[i-1] + aligned_seq1;
            aligned_seq2 = '-' + aligned_seq2;
            i--;
        }
    }

    int start1 = i;
    int start2 = j;

    // --- Build match_line
    std::string match_line = "";
    for (size_t k = 0; k < aligned_seq1.size(); ++k) {
        if (aligned_seq1[k] == aligned_seq2[k])
            match_line += "|";
        else if (aligned_seq1[k] == '-' || aligned_seq2[k] == '-')
            match_line += " ";
        else
            match_line += "*";
    }

    // --- Return result
    AlignmentResult result;
    result.aligned_seq1 = aligned_seq1;
    result.aligned_seq2 = aligned_seq2;
    result.match_line = match_line;
    result.start1 = start1;
    result.start2 = start2;
    result.score = max_score;
    return result;
}
