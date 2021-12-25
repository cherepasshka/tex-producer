#pragma once

#include <iostream>
#include <vector>

#include "fraction.h"

using Matrix = std::vector<std::vector<Fraction<int64_t>>>;
class LatexPinter {
private:
    int matricies_in_block;
    static const int MAX_MATRICIES_IN_BLOCK = 3;
    std::ostream& out;

public:
    LatexPinter(std::ostream& o) : out(o) {
        matricies_in_block = 0;
        // out << "In constructor\n";
        OpenBlock();
    }
    ~LatexPinter() {
        if (matricies_in_block == 0) {
            CloseBlock();
        }
    }
    void PrintMatrix(const Matrix& m) {
        out << "\\begin{pmatrix} \n";
        for (int i = 0; i < m.size(); ++i) {
            for (int j = 0; j < m[i].size(); ++j) {
                out << m[i][j];
                if (j + 1 < m[i].size()) {
                    out << " & ";
                } else {
                    out << " \\\\ ";
                }
            }
            out << "\n";
        }
        out << "\\end{pmatrix} \n";
        ++matricies_in_block;
        if (matricies_in_block == MAX_MATRICIES_IN_BLOCK) {
            matricies_in_block = 0;
            CloseBlock();
            OpenBlock();
        }
    }
    void PrintTransformation() {
        out << "\\Longrightarrow \n";
    }
    void PrintNewLine() {
        out << "\\\\\n";
    }
    void CloseBlock() {
        out << "$$\n";
    }
    void OpenBlock() {
        out << "$$\n";
    }
};