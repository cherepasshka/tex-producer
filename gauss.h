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
    out << "\\end{pmatrix} \n";
}
void PrintTransformation(std::ostream& out) {
    out << "\\Longrightarrow \n";
}
void PrintNewLine(std::ostream& out) {
    out << "\\\\\n";
}
void CloseBlock(std::ostream& out) {
    out << "$$\n";
}
void OpenBlock(std::ostream& out) {
    out << "$$\n";
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
int GetNonZeroRow(const Matrix& m, int start, int column) {
    int row = m.size();
    for (int i = start; i < m.size(); ++i) {
        if (m[i][column] != 0) {
            row = i;
            break;
        }
    }
    return row;
}
void ToSteppedView(std::ostream& out, Matrix& m /*, const Column& b*/) {
    OpenBlock(out);
    int var_pos = 0;
    int prints = 0;
    for (int i = 0; i < m[0].size(); ++i) {
        int non_zero = GetNonZeroRow(m, var_pos, i);
        if (non_zero == m.size()) {
            ++var_pos;
            continue;
        }

        SwapRows(m, non_zero, var_pos);
        PrintTransformation(out);
        PrintMatrix(out, m);
        prints++;
        if (prints == 3) {
            CloseBlock(out);
            OpenBlock(out);
            prints = 0;
        }
        var_pos++;
        for (int j = var_pos; j < m.size(); ++j) {
            if (m[j][i] == 0) {
                continue;
            }
            SubstractRows(m, j, var_pos - 1, m[j][i] / m[var_pos - 1][i]);
            PrintTransformation(out);
            PrintMatrix(out, m);
            prints++;
            if (prints == 3) {
                CloseBlock(out);
                OpenBlock(out);
                prints = 0;
            }
        }
    }
    std::cerr << prints << "\n";
    if (prints == 0) {
        CloseBlock(out);
    }
}