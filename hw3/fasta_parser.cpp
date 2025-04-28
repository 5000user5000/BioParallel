#include "fasta_parser.hpp"
#include <fstream>
#include <sstream>

std::string read_fasta_sequence(const std::string& filename) {
    std::ifstream file(filename);
    std::string line, sequence;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '>') continue; // Skip FASTA header
        sequence += line;
    }

    return sequence;
}
