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
    for (size_t i = 0; i < m[0].size(); ++i) {
        std::swap(m[i1][i], m[i2][i]);
    }
}
void MultiplyRow(Matrix& m, size_t j, const Fraction<int64_t>& alpha) {
    for (size_t i = 0; i < m[0].size(); ++i) {
        m[j][i] = m[j][i] * alpha;
    }
}
void SubstractRows(Matrix& m, size_t i1, size_t i2, const Fraction<int64_t>& alpha) {
    for (size_t i = 0; i < m[0].size(); ++i) {
        m[i1][i] = m[i1][i] - alpha * m[i2][i];
    }
}
size_t GetNonZeroRow(const Matrix& m, int start, int column) {
    size_t row = m.size();
    for (size_t i = start; i < m.size(); ++i) {
        if (m[i][column] != 0) {
            row = i;
            break;
        }
    }
    return row;
}
void ToSteppedView(std::ostream& out, Matrix& m, size_t variable_columns, bool improved = false) {
    LatexPinter printer(out);
    int variable_position = 0;
    size_t non_zero = m.size();
    size_t matrix_columns = m[0].size() - variable_columns;
    Fraction<int64_t> alpha = 0;
    printer.PrintMatrix(m, matrix_columns);
    for (size_t i = 0; i < matrix_columns; ++i) {
        non_zero = GetNonZeroRow(m, variable_position, i);
        if (non_zero == m.size()) {
            ++variable_position;
            continue;
        }
        if (non_zero != variable_position) {
            SwapRows(m, non_zero, variable_position);
            printer.PrintTransformation(TransformationSwap(non_zero, variable_position));
            printer.PrintMatrix(m, m[0].size() - variable_columns);
        }
        alpha = Fraction<int64_t>(1) / m[variable_position][i];
        if (alpha != 1) {
            MultiplyRow(m, variable_position, alpha);
            printer.PrintTransformation(TransformationMul(variable_position, alpha));
            printer.PrintMatrix(m, m[0].size() - variable_columns);
        }
        ++variable_position;
        for (size_t j = improved ? 0 : variable_position; j < m.size(); ++j) {
            if (m[j][i] == 0 || j == variable_position - 1) {
                continue;
            }
            alpha = m[j][i] / m[variable_position - 1][i];
            SubstractRows(m, j, variable_position - 1, alpha);
            printer.PrintTransformation(TransformationSub(j, variable_position - 1, alpha));
            printer.PrintMatrix(m, m[0].size() - variable_columns);
        }
    }
}
void Gaussian(std::ostream& out, const Matrix& input_matrix, const Column& input_b) {
    Matrix m = input_matrix;
    for (size_t i = 0; i < input_b.size(); ++i) {
        m[i].push_back(input_b[i]);
    }
    ToSteppedView(out, m, 1, true);
    
    LatexPinter printer(out);
    for(int i = m.size() - 1; i >=0; --i) {
        bool zero_row = true;
        for(int j = 0; j + 1 < m[i].size(); ++j){
            if(m[i][j] != 0) {
                zero_row = false;
                break;
            }
        }
        if(zero_row && m[i].back() != 0) {
            std::cerr << i << " row\n";
            std::cerr << m[i].back() << "\n";
            printer.PrintText("No solution");
            return;
        }
    }
    std::vector<std::string> exprs(m.size());
    for(int i = 0; i < m.size(); ++i) {
        int main_var = m.size();
        for(int j = 0; j < input_matrix[i].size(); ++j) {
            if(m[i][j] != 0) {
                main_var = j;
                break;
            }
        }
        if(main_var == m.size()) {
            continue;
        }
        exprs[main_var] = m[i].back().ToStr();
        for(int j = main_var + 1; j < input_matrix[i].size(); ++j) {
            Fraction<int64_t> alpha = -m[i][j];
            if(alpha != 0) {
                exprs[main_var] += " + " + alpha.ToStr() + "\\cdot x_{" + std::to_string(j + 1) + "}" ;
            }
        }
    }
    for(int i = 0; i < exprs.size(); ++i) {
        if(exprs[i].empty()) {
            exprs[i] = "x_{" + std::to_string(i + 1) + "}";
        }
    }
    printer.PrintText("Answer: ");
    printer.PrintLetteredMatrix(m.size(), 1, 'x');
    printer.PrintText("=");
    printer.PrintExpressiondColumn(m.size(), exprs);
}