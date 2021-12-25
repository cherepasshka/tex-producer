#pragma once

#include <cstdint>
#include <iomanip>

// using FractionType = int64_t ;

template <class FractionType>
class Fraction {
private:
    FractionType numerator, denominator;
    FractionType Gcd(FractionType a, FractionType b) {
        if (b == 0) {
            return a;
        } else {
            return Gcd(b, a % b);
        }
    }
    Fraction Validate() {
        FractionType gcd = Gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

public:
    Fraction(FractionType num = 0, FractionType den = 1) {
        numerator = num;
        denominator = den;
    }
    Fraction operator+(const Fraction& other) const {
        return Fraction(numerator * other.denominator + denominator * other.numerator,
                        denominator * other.denominator)
            .Validate();
    }
    Fraction operator-() const {
        return Fraction(-numerator, denominator);
    }
    Fraction operator-(const Fraction& other) const {
        return *this + (-other);
    }
    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator).Validate();
    }
    Fraction operator/(const Fraction& other) const {
        return Fraction(numerator * other.denominator, denominator * other.numerator).Validate();
    }
    bool operator==(const Fraction& other) const {
        return numerator * other.denominator == denominator * other.numerator;
    }
    bool operator!=(const Fraction& other) const {
        return numerator * other.denominator != denominator * other.numerator;
    }
    // template<class T>
    friend std::ostream& operator<<(std::ostream& out, const Fraction& fraction);
};

std::ostream& operator<<(std::ostream& out, const Fraction<int64_t>& fraction) {
    if (fraction.denominator == 1 || fraction.numerator == 0) {
        out << fraction.numerator;
    } else {
        out << fraction.numerator << "/" << fraction.denominator;
    }
    return out;
}