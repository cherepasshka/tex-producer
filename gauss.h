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
    int variable_position = 0;
    int non_zero = m.size();
    Fraction<int64_t> alpha = 0;
    for (int i = 0; i < m[0].size(); ++i) {
        non_zero = GetNonZeroRow(m, variable_position, i);
        if (non_zero == m.size()) {
            ++variable_position;
            continue;
        }
        SwapRows(m, non_zero, variable_position);
        if (non_zero != variable_position) {
            printer.PrintTransformation(TransformationSwap(non_zero, variable_position));
            printer.PrintMatrix(m);
        }

        ++variable_position;
        for (int j = variable_position; j < m.size(); ++j) {
            if (m[j][i] == 0) {
                continue;
            }
            alpha = m[j][i] / m[variable_position - 1][i];
            SubstractRows(m, j, variable_position - 1, alpha);
            printer.PrintTransformation(TransformationSub(j, variable_position - 1, alpha));
            printer.PrintMatrix(m);
        }
    }
}