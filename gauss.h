#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "fraction.h"
#include "latex_printer.h"
#include "matrix.h"

// using Matrix = std::vector<std::vector<Fraction<int64_t>>>;
using Column = std::vector<Fraction<int64_t>>;
using Rational = Fraction<int64_t>;

void SwapRows(Matrix<Rational>& m, size_t i1, size_t i2) {
    for (size_t i = 0; i < m.ColumnCnt(); ++i) {
        std::swap(m[i1][i], m[i2][i]);
    }
}
void MultiplyRow(Matrix<Rational>& m, size_t j, const Fraction<int64_t>& alpha) {
    for (size_t i = 0; i < m.ColumnCnt(); ++i) {
        m[j][i] = m[j][i] * alpha;
    }
}
void SubstractRows(Matrix<Rational>& m, size_t i1, size_t i2, const Fraction<int64_t>& alpha) {
    for (size_t i = 0; i < m.ColumnCnt(); ++i) {
        m[i1][i] = m[i1][i] - alpha * m[i2][i];
    }
}
size_t GetNonZeroRow(const Matrix<Rational>& m, int start, int column) {
    for (size_t i = start; i < m.RowCnt(); ++i) {
        if (m[i][column] != 0) {
            return i;
        }
    }
    return m.RowCnt();
}
void ToSteppedView(std::ostream& out, Matrix<Rational>& m, size_t variable_columns,
                   bool improved = false) {
    LatexPinter printer(out);
    size_t non_zero_row = m.RowCnt(), start_row = 0;
    size_t matrix_columns = m.ColumnCnt() - variable_columns;
    Fraction<int64_t> alpha = 0;

    printer.PrintMatrix(m, matrix_columns);
    for (size_t i = 0; i < matrix_columns; ++i) {
        non_zero_row = GetNonZeroRow(m, start_row, i);
        if (non_zero_row == m.RowCnt()) {
            continue;
        }

        if (non_zero_row != start_row) {
            SwapRows(m, non_zero_row, start_row);
            printer.PrintTransformation(TransformationSwap(non_zero_row, start_row));
            printer.PrintMatrix(m, m.ColumnCnt() - variable_columns);
        }
        alpha = Fraction<int64_t>(1) / m[start_row][i];
        if (alpha != 1) {
            MultiplyRow(m, start_row, alpha);
            printer.PrintTransformation(TransformationMul(start_row, alpha));
            printer.PrintMatrix(m, m.ColumnCnt() - variable_columns);
        }
        for (size_t j = improved ? 0 : start_row; j < m.RowCnt(); ++j) {
            if (m[j][i] == 0 || j == start_row) {
                continue;
            }
            alpha = m[j][i];  // / m[start_row][i];
            SubstractRows(m, j, start_row, alpha);
            printer.PrintTransformation(TransformationSub(j, start_row, alpha));
            printer.PrintMatrix(m, m.ColumnCnt() - variable_columns);
        }
        ++start_row;
    }
}

void GetMajorMinorVariables(const Matrix<Rational>& m, std::set<size_t>& major,
                            std::set<size_t>& minor, std::vector<size_t>& row) {
    for (size_t i = 0; i < m.RowCnt(); ++i) {
        for (size_t j = 0; j < m.ColumnCnt(); ++j) {
            if (m[i][j] != 0) {
                major.insert(j);
                row[j] = i;
                break;
            }
        }
    }
    for (size_t i = 0; i < m.ColumnCnt(); ++i) {
        if (major.find(i) == major.end()) {
            minor.insert(i);
        }
    }
}
void Gaussian(std::ostream& out, const Matrix<Rational>& input_matrix, const Column& input_b) {
    LatexPinter printer(out);
    Matrix<Rational> m = input_matrix;
    for (size_t i = 0; i < input_b.size(); ++i) {
        m[i].push_back(input_b[i]);
    }
    ToSteppedView(out, m, 1, true);
    Matrix<Rational> stepped = input_matrix;
    Column b = input_b;
    for (size_t i = 0; i < stepped.RowCnt(); ++i) {
        for (size_t j = 0; j < stepped.ColumnCnt(); ++j) {
            stepped[i][j] = m[i][j];
        }
        b[i] = m[i].back();
    }
    for (int i = stepped.RowCnt() - 1; i >= 0; --i) {
        bool is_zero_row = true;
        for (size_t j = 0; j < stepped.ColumnCnt(); ++j) {
            if (stepped[i][j] != 0) {
                is_zero_row = false;
                break;
            }
        }
        if (is_zero_row && b[i] != 0) {
            printer.PrintText("No solution");
            return;
        }
    }
    std::vector<std::string> exprs(stepped.ColumnCnt());
    std::set<size_t> major, minor;
    std::vector<size_t> row(stepped.ColumnCnt(), 0);
    GetMajorMinorVariables(stepped, major, minor, row);
    for (auto i : minor) {
        exprs[i] = "x_{" + std::to_string(i + 1) + "} ";
    }
    for (auto major_var : major) {
        exprs[major_var] = b[row[major_var]].ToStr();
        for (auto minor_var : minor) {
            if (stepped[row[major_var]][minor_var] == 0) {
                continue;
            }
            // if (stepped[row[major_var]][minor_var] > 0) {
            //     exprs[major_var] += " + ";
            // }
            exprs[major_var] += " - " + stepped[row[major_var]][minor_var].ToStr() + "x_{" +
                                std::to_string(minor_var + 1) + "}";
        }
    }

    printer.PrintText("Answer: ");
    printer.PrintLetteredMatrix(stepped.ColumnCnt(), 1, 'x');
    printer.PrintText("=");
    printer.PrintExpressiondColumn(exprs.size(), exprs);
}

std::vector<Column> GetFundametalSolutionSystem(std::ostream& out,
                                                const Matrix<Rational>& input_matrix) {
    LatexPinter printer(out);
    Matrix<Rational> m = input_matrix;
    ToSteppedView(out, m, 0, true);
    std::set<size_t> major, minor;
    std::vector<size_t> row(m.ColumnCnt());
    GetMajorMinorVariables(m, major, minor, row);
    Column basis_vector(minor.size() + major.size());
    std::vector<Column> basis;
    printer.PrintText("Fundamental solutions system: ");
    for (auto minor_var : minor) {
        for (size_t i = 0; i < basis_vector.size(); ++i) {
            basis_vector[i] = 0;
        }
        basis_vector[minor_var] = 1;
        for (auto main_var : major) {
            basis_vector[main_var] = -m[row[main_var]][minor_var];
        }
        basis.push_back(basis_vector);
        printer.PrintMatrix(Matrix<Rational>({basis_vector}), basis_vector.size() + 1);
    }
    return basis;
}