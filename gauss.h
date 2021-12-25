#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "fraction.h"
#include "latex_printer.h"

using Matrix = std::vector<std::vector<Fraction<int64_t>>>;
using Column = std::vector<Fraction<int64_t>>;

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
    LatexPinter printer(out);
    int var_pos = 0;
    int prints = 0;
    for (int i = 0; i < m[0].size(); ++i) {
        int non_zero = GetNonZeroRow(m, var_pos, i);
        if (non_zero == m.size()) {
            ++var_pos;
            continue;
        }
        SwapRows(m, non_zero, var_pos);
        printer.PrintTransformation();
        printer.PrintMatrix(m);

        var_pos++;
        for (int j = var_pos; j < m.size(); ++j) {
            if (m[j][i] == 0) {
                continue;
            }
            SubstractRows(m, j, var_pos - 1, m[j][i] / m[var_pos - 1][i]);
            printer.PrintTransformation();
            printer.PrintMatrix(m);
        }
    }
}