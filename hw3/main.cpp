#include "fasta_parser.hpp"
#include "align_sw.hpp"
#include "align_sw_simd.hpp"
#include "align_sw_cuda.hpp" 
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

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

        for (char c : chunk1) if (c != '-') ++idx1;
        for (char c : chunk2) if (c != '-') ++idx2;

        size_t chunk_end1 = idx1;
        size_t chunk_end2 = idx2;

        std::stringstream padding;
        padding << "Seq1: " << std::setw(4) << chunk_start1 << "  ";
        std::string match_prefix(padding.str().length(), ' ');

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

    std::string seq1 = read_fasta_sequence(argv[1]);
    std::string seq2 = read_fasta_sequence(argv[2]);

    // 標準 Scalar 計時
    auto start = std::chrono::high_resolution_clock::now();
    AlignmentResult result_scalar = smith_waterman(seq1, seq2);
    auto end = std::chrono::high_resolution_clock::now();
    double time_scalar = std::chrono::duration<double>(end - start).count();

    std::cout << "\nScalar Alignment:\n";
    print_alignment(result_scalar);

    // SIMD 計時
    auto start_simd = std::chrono::high_resolution_clock::now();
    AlignmentResult result_simd = smith_waterman_simd(seq1, seq2);
    auto end_simd = std::chrono::high_resolution_clock::now();
    double time_simd = std::chrono::duration<double>(end_simd - start_simd).count();

    std::cout << "\nSIMD Alignment:\n";
    print_alignment(result_simd);

    std::cout << "\nSIMD Speedup (vs Scalar): " << (time_scalar / time_simd) << "X\n";

    // CUDA 計時
    auto start_cuda = std::chrono::high_resolution_clock::now();
    AlignmentResult result_cuda = smith_waterman_cuda(seq1, seq2);
    auto end_cuda = std::chrono::high_resolution_clock::now();
    double time_cuda = std::chrono::duration<double>(end_cuda - start_cuda).count();

    std::cout << "\nCUDA Alignment:\n";
    print_alignment(result_cuda);

    std::cout << "\nCUDA Speedup (vs Scalar): " << (time_scalar / time_cuda) << "X\n";
    std::cout << "CUDA Speedup (vs SIMD): " << (time_simd / time_cuda) << "X\n";

    return 0;
}
