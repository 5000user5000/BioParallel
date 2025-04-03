#include "fasta_parser.hpp"
#include "align_sw.hpp"
#include <iostream>
#include <chrono>
#include "align_sw_simd.hpp"
#include <string>

void print_alignment(const AlignmentResult& result, size_t width = 60) {
    std::cout << "Score: " << result.score << "\n\n";

    const std::string& seq1 = result.aligned_seq1;
    const std::string& seq2 = result.aligned_seq2;
    const std::string& match = result.match_line;

    size_t len = seq1.size();
    for (size_t i = 0; i < len; i += width) {
        size_t chunk_len = std::min(width, len - i);

        std::string chunk1 = seq1.substr(i, chunk_len);
        std::string chunk2 = seq2.substr(i, chunk_len);
        std::string chunkm = match.substr(i, chunk_len);

        // Print with perfect vertical alignment
        std::cout << "Seq1:    " << chunk1 << "\n";
        std::cout << "         " << chunkm << "\n";
        std::cout << "Seq2:    " << chunk2 << "\n\n";
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
