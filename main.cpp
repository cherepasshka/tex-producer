#include <fstream>
#include <iostream>

#include "fwd_declare.h"
#include "gauss.h"
#include "latex_printer.h"
#include "matrix.h"
#include "simple_parser.h"

void Help() {
    std::cerr << "Usage sample: ./main.o input.txt.\nWhere input.txt contains matrix";
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        Help();
        return 1;
    }
    std::ifstream in(argv[1], std::ios_base::in);
    std::string line, word;
    Matrix<Rational> coef_matrix;
    Matrix<Rational> B;
    std::vector<std::string> modes;
    bool were_modes = false;
    while (std::getline(in, line)) {
        std::stringstream sin(line);
        sin >> word;
        word = ToLower(word);
        if (word == "mode" || word == "modes") {
            while (sin >> word) {
                word = ToLower(word);
                modes.push_back(word);
            }
            were_modes = true;
        } else if (!were_modes) {
            auto row = GetRow(line);
            if (row.size() == 0) {
                continue;
            }
            coef_matrix.AddRow(row);
        } else {
            auto row = GetRow(line);
            if (row.size() == 0) {
                continue;
            }
            B.AddRow(row);
        }
    }
    for (auto &mode : modes) {
        if (mode == "g") {
            Gaussian(std::cout, coef_matrix, B);
        }
        if (mode == "f") {
            GetFundametalSolutionSystem(std::cout, coef_matrix);
        }
    }
    return 0;
}