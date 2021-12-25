#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "fraction.h"

using Matrix = std::vector<std::vector<Fraction<int64_t>>>;

std::string TransformationSub(int row1, int row2, const Fraction<int64_t>& alpha) {
    return "[" + std::to_string(row1 + 1) + "] - " + alpha.ToStr() + "  [" +
           std::to_string(row2 + 1) + "]";
}
std::string TransformationSwap(int row1, int row2) {
    return "\\text{swap}([" + std::to_string(row1 + 1) + "], [" + std::to_string(row2 + 1) + "])";
}

class LatexPinter {
private:
    int matricies_in_block;
    static const int MAX_MATRICIES_IN_BLOCK = 3;
    std::ostream& out;

public:
    LatexPinter(std::ostream& o) : out(o) {
        matricies_in_block = 0;
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
    void PrintTransformation(const std::string transformation) {
        out << "\\overset{" << transformation << "}{\\longrightarrow} \n";
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