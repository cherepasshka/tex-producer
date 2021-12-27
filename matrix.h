#pragma once
#include <cmath>
#include <vector>

template <class T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix;

public:
    Matrix() {
    }
    Matrix(const std::vector<std::vector<T>>& m) : matrix(m) {
    }
    Matrix(size_t row_cnt, size_t column_cnt) : matrix(row_cnt, std::vector<T>(column_cnt)) {
    }
    // template<class Inner>
    // Matrix(const std::initializer_list<std::initializer_list<Inner>>& list) : matrix(list) {
    // }
    std::vector<T>& operator[](size_t ind) {
        return matrix[ind];
    }
    const std::vector<T>& operator[](size_t ind) const {
        return matrix[ind];
    }
    // size_t size() const {
    //     return matrix.size();
    // }
    size_t RowCnt() const {
        return matrix.size();
    }
    size_t ColumnCnt() const {
        if (matrix.size() == 0) {
            return 0;
        }
        return matrix[0].size();
    }
    Matrix Transposed() const {
        std::vector<std::vector<T>> transposed(ColumnCnt(), std::vector<T>(RowCnt()));
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                transposed[j][i] = matrix[i][j];
            }
        }
        return transposed;
    }
    Matrix SubMatrix(size_t row_start, size_t row_end, size_t clmn_start, size_t clmn_end) const {
        Matrix res(row_end - row_start, clmn_end - clmn_start);
        for (size_t i = row_start; i < row_end; ++i) {
            for (size_t j = clmn_start; j < clmn_end; ++j) {
                res[i - row_start][j - clmn_start] = matrix[i][j];
            }
        }
        return res;
    }
};