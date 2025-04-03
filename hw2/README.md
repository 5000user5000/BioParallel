# HW02 - Smith-Waterman Sequence Alignment with XSIMD Acceleration

##  Overview

This project implements the Smith-Waterman algorithm for local sequence alignment in C++, including:

- A **scalar (non-SIMD)** version
- An **SIMD-accelerated version using [XSIMD](https://github.com/xtensor-stack/xsimd)**

Both versions are benchmarked, and a simplified BLAST-like alignment output is generated for comparison. The speedup of the SIMD version over the scalar version is also reported.


## 📁 File Structure

```
.
├── Makefile                 # Build configuration
├── main.cpp                 # Main program logic
├── align_sw.hpp / .cpp      # Scalar Smith-Waterman implementation
├── align_sw_simd.hpp / .cpp # SIMD Smith-Waterman using XSIMD
├── fasta_parser.hpp / .cpp  # FASTA file parser
├── seq1.fasta               # Sample input sequence 1
├── seq2.fasta               # Sample input sequence 2
└── README.md                # You're here
```


## ⚙️ Build Instructions

### ✅ Requirements

- C++17 or later
- [XSIMD](https://github.com/xtensor-stack/xsimd) header-only library

Clone XSIMD if needed:
```bash
git clone https://github.com/xtensor-stack/xsimd.git
```

Update the `XSIMD_INCLUDE` path in the `Makefile` if necessary.

### 🔧 Build

To compile the project:
```bash
make
```

To clean up object files:
```bash
make clean
```


## Usage

### Command Line

```bash
./sw_align <seq1.fasta> <seq2.fasta>
```

Example:
```bash
./sw_align seq1.fasta seq2.fasta
```

### Make Test

You can also run the test using:
```bash
make test
```

## 📤 Output Format

After running, you will see:

- **Alignment score**
- **Simplified BLAST-like alignment display**
- **Reported speedup (SIMD vs scalar)**

Example output:

```
Score: 78
Seq1:  453    CCAATGCCACAAAACATCTGTCTCTAACTGGTG--TGTGTGT    492
              |||  ||| ||||  |||||| | ||| |||||  |*|||||
Seq2:   17    CCA--GCC-CAAA--ATCTGT-TTTAA-TGGTGGATTTGTGT    51

Speedup: 2.3X
```

Legend:
- `|` : match
- `*` : mismatch
- ` ` : gap/indel (`-` added in alignment)


##  Notes

- The SIMD implementation only accelerates the scoring matrix computation; traceback is reused from the scalar implementation for correctness and simplicity.
- The program assumes that FASTA sequences are single-line and contain no line breaks (per assignment instructions).
- You can tune match/mismatch/gap scores by editing the function arguments in `main.cpp`.



##  Author

This project was completed as part of **Homework 2** for the course on bioinformatics/systems programming.

If you encounter issues running the code, please check:
- That XSIMD is correctly included in the build path
- That your sequences are valid and in FASTA format