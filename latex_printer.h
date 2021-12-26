#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "fraction.h"

using Matrix = std::vector<std::vector<Fraction<int64_t>>>;
std::string Romanian(size_t num) {
    if (num == 1) {
        return "\\mathbb{I} ";
    } else if (num == 2) {
        return "\\mathbb{II} ";
    } else if (num == 3) {
        return "\\mathbb{III} ";
    } else if (num == 4) {
        return "\\mathbb{IV} ";
    } else if (num == 5) {
        return "\\mathbb{V} ";
    } else if (num == 6) {
        return "\\mathbb{VI} ";
    } else if (num == 7) {
        return "\\mathbb{VII} ";
    } else if (num == 8) {
        return "\\mathbb{VIII} ";
    }

    throw std::runtime_error("Wow.... you have more than 8 rows in your matrix....");
}
std::string TransformationSub(size_t row1, size_t row2, Fraction<int64_t> alpha) {
    std::string operation = " - ";
    if (alpha < 0) {
        alpha = -alpha;
        operation = " + ";
    }
    return Romanian(row1 + 1) + operation + alpha.ToStr() + Romanian(row2 + 1);
}
std::string TransformationSwap(size_t row1, size_t row2) {
    return "\\text{swap}(" + Romanian(row1 + 1) + ", " + Romanian(row2 + 1) + ")";
}
std::string TransformationMul(size_t row, const Fraction<int64_t>& alpha) {
    return alpha.ToStr() + "\\cdot" + Romanian(row + 1);
}
class LatexPinter {
private:
    int matricies_in_block;
    static const int MAX_MATRICIES_IN_BLOCK = 2;
    std::ostream& out;
    bool is_full_document, is_block_open;

public:
    LatexPinter(std::ostream& o, bool is_full_doc = false) : out(o) {
        matricies_in_block = 0;
        is_block_open = false;
        is_full_document = is_full_doc;
        if (is_full_document) {
            AddDocumentPrologue();
        }
        // OpenBlock();
    }
    ~LatexPinter() {
        if (is_block_open) {
            CloseBlock();
        }
        if (is_full_document) {
            AddDocumentEpilogue();
        }
    }
    void PrintText(const std::string& text) {
        if (!is_block_open) {
            OpenBlock();
        }
        out << " \\text{" << text << "} \n";
    }
    void PrintExpressiondColumn(size_t row_cnt, const std::vector<std::string>& expressions) {
        if (!is_block_open) {
            OpenBlock();
        }
        out << "\\left(\\begin{array}{l} \n";
        for (int i = 0; i < row_cnt; ++i) {
            out << "\t " << expressions[i] << "\\\\";
            out << "\n";
        }
        out << "\\end{array}\\right) \n";
        ++matricies_in_block;
        if (matricies_in_block == MAX_MATRICIES_IN_BLOCK) {
            matricies_in_block = 0;
            CloseBlock();
        }
    }
    void PrintLetteredMatrix(size_t row_cnt, size_t column_cnt, char letter) {
        if (!is_block_open) {
            OpenBlock();
        }
        out << "\\left(\\begin{array}{";
        for (int i = 0; i < column_cnt; ++i) {
            out << "l";
        }
        out << "} \n";
        for (int i = 0; i < row_cnt; ++i) {
            out << "\t";
            for (int j = 0; j < column_cnt; ++j) {
                if (column_cnt == 1) {
                    out << letter << "_{" << i + 1 << "} ";
                } else {
                    out << letter << "_{" << i + 1 << ", " << j + 1 << "} ";
                }
                if (j + 1 < column_cnt) {
                    out << " & ";
                } else {
                    out << " \\\\ ";
                }
            }
            out << "\n";
        }
        out << "\\end{array}\\right) \n";
        ++matricies_in_block;
        if (matricies_in_block == MAX_MATRICIES_IN_BLOCK) {
            matricies_in_block = 0;
            CloseBlock();
        }
    }
    void PrintMatrix(const Matrix& m, size_t line_ind) {
        if (!is_block_open) {
            OpenBlock();
        }
        out << "\\left(\\begin{array}{";
        for (size_t i = 0; i < m[0].size(); ++i) {
            if (i == line_ind) {
                out << "|";
            }
            out << "l";
        }
        out << "} \n";
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
        out << "\\end{array}\\right) \n";
        ++matricies_in_block;
        if (matricies_in_block == MAX_MATRICIES_IN_BLOCK) {
            matricies_in_block = 0;
            CloseBlock();
        }
    }
    void PrintTransformation(const std::string transformation) {
        if (!is_block_open) {
            OpenBlock();
        }
        out << "\\overset{" << transformation << "}{\\longrightarrow} \n";
    }
    void PrintNewLine() {
        if (!is_block_open) {
            OpenBlock();
        }
        out << "\\\\\n";
    }
    void CloseBlock() {
        out << "$$\n";
        is_block_open = false;
    }
    void OpenBlock() {
        out << "$$\n";
        is_block_open = true;
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
        out << "\\author{Author here} \n";
        out << "\\title{Some title} \n";
        out << "\\date{\\today} \n";
        out << "\\begin{document} \n";
        out << "\\maketitle \n\n";
    }
    void AddDocumentEpilogue() {
        out << "\\end{document} \n";
    }
};