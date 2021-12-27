#include <iostream>
#include <fstream>

#include "fwd_declare.h"
#include "gauss.h"
#include "latex_printer.h"
#include "simple_parser.h"
#include "matrix.h"

void Help() {
    std::cerr << "Usage sample: ./main.o input.txt.\nWhere input.txt contains matrix";
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        Help();
        return 1;
    }
    std::ifstream in(argv[1], std::ios_base::in);
    std::string line;
    Matrix<Rational> matrix;

    while(getline(in, line)) {
        matrix.AddRow(GetRow(line));
    }
    Gaussian(std::cout, matrix, {1, 2, 3});
    GetFundametalSolutionSystem(std::cout, matrix);
    return 0;
}