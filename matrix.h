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
};