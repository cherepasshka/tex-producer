#include <cstdint>
#include <iomanip>

typedef int64_t FractionType;

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
        return Fraction(numerator * other.denominator + other.denominator * numerator,
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
    friend std::ostream& operator<<(std::ostream& out, const Fraction& fraction);
};

std::ostream& operator<<(std::ostream& out, const Fraction& fraction) {
    out << fraction.numerator << "/" << fraction.denominator;
    return out;
}