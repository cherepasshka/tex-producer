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
    bool is_full_document;

public:
    LatexPinter(std::ostream& o, bool is_full_doc = false) : out(o) {
        matricies_in_block = 0;
        is_full_document = is_full_doc;
        if (is_full_document) {
            AddDocumentPrologue();
        }
        OpenBlock();
    }
    ~LatexPinter() {
        if (matricies_in_block == 0) {
            CloseBlock();
        }
        if (is_full_document) {
            AddDocumentEpilogue();
        }
    }
    void PrintMatrix(const Matrix& m) {
        out << "\\begin{pmatrix} \n";
        for (int i = 0; i < m.size(); ++i) {
            out << "\t";
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
    void AddDocumentPrologue() {
        out << "\\documentclass[a4paper,12pt]{article}\n\n";
        std::vector<std::string> packages = {"\\usepackage{cmap}",
                                             "\\usepackage[warn]{mathtext}",
                                             "\\usepackage[T2A]{fontenc}",
                                             "\\usepackage[utf8]{inputenc}",
                                             "\\usepackage[english,russian]{babel}",
                                             "\\usepackage{amsfonts}",
                                             "\\usepackage{amsmath}"};
        for (const auto package : packages) {
            out << package << "\n";
        }
        out << "\n";
        out << "\\author{Полуденный кек} \n";
        out << "\\title{Идентифицирую себя как сгоревшую спичку} \n";
        out << "\\date{\\today} \n";
        out << "\\begin{document} \n";
        out << "\\maketitle \n\n";
    }
    void AddDocumentEpilogue() {
        out << "\\end{document} \n";
    }
};