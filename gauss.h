#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "fraction.h"
#include "fwd_declare.h"
#include "latex_printer.h"
#include "matrix.h"

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

    // return;
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
bool IsZeroRow(const Matrix<Rational>& m, size_t row) {
    for (size_t i = 0; i < m.ColumnCnt(); ++i) {
        if (m[row][i] != 0) {
            return false;
        }
    }
    return true;
}
void Gaussian(std::ostream& out, const Matrix<Rational>& input_matrix,
              const Matrix<Rational>& input_b) {
    if (input_matrix.RowCnt() != input_b.RowCnt()) {
        std::cerr << input_b.RowCnt() << " " << input_matrix.RowCnt() << "\n";
        throw std::runtime_error("Matrices have a different number of rows");
    }
    LatexPinter printer(out);
    Matrix<Rational> m = input_matrix;
    for (size_t i = 0; i < input_b.RowCnt(); ++i) {
        for (size_t j = 0; j < input_b.ColumnCnt(); ++j) {
            m[i].push_back(input_b[i][j]);
        }
    }
    ToSteppedView(out, m, input_b.ColumnCnt(), true);
    Matrix<Rational> stepped = m.SubMatrix(0, input_matrix.RowCnt(), 0, input_matrix.ColumnCnt());
    Matrix<Rational> b = m.SubMatrix(0, m.RowCnt(), input_matrix.ColumnCnt(), m.ColumnCnt());
    for (int i = stepped.RowCnt() - 1; i >= 0; --i) {
        if (IsZeroRow(stepped, i) && !IsZeroRow(b, i)) {
            printer.PrintText("No solution");
            return;
        }
    }
    Matrix<std::string> answ(input_matrix.ColumnCnt(), input_b.ColumnCnt());
    std::set<size_t> major, minor;
    std::vector<size_t> row(stepped.ColumnCnt(), 0);
    GetMajorMinorVariables(stepped, major, minor, row);
    for (size_t j = 0; j < answ.ColumnCnt(); ++j) {
        for (auto i : minor) {
            answ[i][j] = "x" + Index(i, j);
        }
    }
    Rational coeficient;
    for (auto major_var : major) {
        for (size_t i = 0; i < answ.ColumnCnt(); ++i) {
            answ[major_var][i] = b[row[major_var]][i].ToStr();
            for (auto minor_var : minor) {
                if (stepped[row[major_var]][minor_var] == 0) {
                    continue;
                }
                if (stepped[row[major_var]][minor_var] < 0) {
                    answ[major_var][i] += " + ";
                    coeficient = -stepped[row[major_var]][minor_var];
                } else {
                    answ[major_var][i] += " - ";
                    coeficient = stepped[row[major_var]][minor_var];
                }
                answ[major_var][i] += coeficient.ToStr() + "x" + Index(minor_var, i);
            }
        }
    }

    printer.PrintText("Answer: ");
    printer.PrintLetteredMatrix(input_matrix.ColumnCnt(), input_b.ColumnCnt(), 'x');
    printer.PrintText("=");
    printer.PrintMatrix(answ, answ.RowCnt());
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
    std::vector<Column> basis(minor.size());
    size_t cur_ind = 0;
    printer.PrintText("Fundamental solutions system: ");
    for (auto minor_var : minor) {
        for (size_t i = 0; i < basis_vector.size(); ++i) {
            basis_vector[i] = 0;
        }
        basis_vector[minor_var] = 1;
        for (auto main_var : major) {
            basis_vector[main_var] = -m[row[main_var]][minor_var];
        }
        basis[cur_ind] = basis_vector;
        printer.PrintMatrix(Matrix<Rational>({basis_vector}).Transposed(), basis_vector.size() + 1);
    }
    return basis;
}