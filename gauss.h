#include "fraction.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>

using Matrix = std::vector<std::vector<Fraction<int64_t>>>;
using Column = std::vector<Fraction<int64_t>>;

void PrintMatrix(std::ostream& out, const Matrix& m) {
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
    out << "\\end{pmatrix} ";
}
void SwapRows(Matrix& m, size_t i1, size_t i2) {
    for (int i = 0; i < m[0].size(); ++i) {
        std::swap(m[i1][i], m[i2][i]);
    }
}
void MultiplyRow(Matrix& m, size_t j, const Fraction<int64_t>& alpha) {
    for (int i = 0; i < m[0].size(); ++i) {
        m[j][i] = m[j][i] * alpha;
    }
}
void SubstractRows(Matrix& m, size_t i1, size_t i2, const Fraction<int64_t>& alpha) {
    for (int i = 0; i < m[0].size(); ++i) {
        m[i1][i] = m[i1][i] - alpha * m[i2][i];
    }
}
void ToSteppedView(const Matrix& m, const Column& b) {
}