#include "fasta_parser.hpp"
#include "align_sw.hpp"
#include <iostream>
#include <chrono>
#include "align_sw_simd.hpp"
#include <string>
#include <iomanip>

void print_alignment(const AlignmentResult& result, size_t width = 60) {
    std::cout << "optimal_alignment_score: " << result.score << "\n\n";

    const std::string& seq1 = result.aligned_seq1;
    const std::string& seq2 = result.aligned_seq2;
    const std::string& match = result.match_line;

    size_t len = seq1.size();
    size_t idx1 = result.start1;
    size_t idx2 = result.start2;

    for (size_t i = 0; i < len; i += width) {
        size_t chunk_len = std::min(width, len - i);

        std::string chunk1 = seq1.substr(i, chunk_len);
        std::string chunk2 = seq2.substr(i, chunk_len);
        std::string chunkm = match.substr(i, chunk_len);

        size_t chunk_start1 = idx1;
        size_t chunk_start2 = idx2;

        // 計算 chunk 結束 index（不計 '-')
        for (char c : chunk1) if (c != '-') ++idx1;
        for (char c : chunk2) if (c != '-') ++idx2;

        size_t chunk_end1 = idx1;
        size_t chunk_end2 = idx2;

        // 計算 match_line 所需的前綴空白：和 Seq1 label 長度一致 + index + 2 個空格
        std::stringstream padding;
        padding << "Seq1: " << std::setw(4) << chunk_start1 << "  ";
        std::string match_prefix(padding.str().length(), ' ');

        // 輸出
        std::cout << "Seq1: " << std::setw(4) << chunk_start1 << "  " << chunk1 << "  " << std::setw(4) << chunk_end1 << "\n";
        std::cout << match_prefix << chunkm << "\n";
        std::cout << "Seq2: " << std::setw(4) << chunk_start2 << "  " << chunk2 << "  " << std::setw(4) << chunk_end2 << "\n\n";
    }
}



int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <seq1.fasta> <seq2.fasta>\n";
        return 1;
    }

    // 讀取 FASTA 檔案
    std::string seq1 = read_fasta_sequence(argv[1]);
    std::string seq2 = read_fasta_sequence(argv[2]);

    // 計時非SIMD版本
    auto start = std::chrono::high_resolution_clock::now();
    AlignmentResult result_scalar = smith_waterman(seq1, seq2);
    auto end = std::chrono::high_resolution_clock::now();
    double time_scalar = std::chrono::duration<double>(end - start).count();

    print_alignment(result_scalar);

    // XSIMD 對齊時間
    auto start_simd = std::chrono::high_resolution_clock::now();
    AlignmentResult result_simd = smith_waterman_simd(seq1, seq2);
    auto end_simd = std::chrono::high_resolution_clock::now();
    double time_simd = std::chrono::duration<double>(end_simd - start_simd).count();

    std::cout << "\nSIMD Alignment:\n";
    print_alignment(result_simd);

    std::cout << "\nSpeedup: " << (time_scalar / time_simd) << "X\n";

    return 0;
}
