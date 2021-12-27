#pragma once

#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#include "fraction.h"
#include "fwd_declare.h"

Rational FromDecimal(const std::string& integer, const std::string& decimal) {
    return Rational(std::stoi(integer)) +
           Rational(std::stoi(decimal), std::pow(10, decimal.size()));
}
Rational ToRational(const std::string& input) {
    size_t pos = input.size();
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '.' || input[i] == '/') {
            pos = i;
            break;
        }
    }
    if (pos == input.size()) {
        return std::stoi(input);
    } else if (input[pos] == '.') {
        return FromDecimal(input.substr(0, pos), input.substr(pos + 1, input.size() - pos - 1));
    }
    return Rational(std::stoi(input.substr(0, pos)),
                    std::stoi(input.substr(pos + 1, input.size() - pos - 1)));
}
std::vector<Rational> GetRow(const std::string& input) {
    std::stringstream sin(input);
    std::vector<Rational> res;
    std::string number;
    while (sin >> number) {
        res.push_back(ToRational(number));
    }
    return res;
}