#ifndef POLYNOMIAL2D_H

#include <array>
#include <initializer_list>
#include <ostream>
#include <set>
#include <string>
#define POLYNOMIAL2D_H

template <typename T>
using initializer_list2D = std::initializer_list<std::initializer_list<T>>;

class Polynomial2D {
private:
    const static size_t MAX_DEG = 256;  // 最大次數

    // degrees in x and y
    std::multiset<size_t> __deg_x, __deg_y;

    // coefficients, coefs[i][j] is the coefficient for x^i * y^j
    std::array<std::array<double, MAX_DEG>, MAX_DEG> coefs;

    inline void update(size_t exp_x, size_t exp_y, double value);

public:
    // Constructors, destructor, and assignments

    Polynomial2D() : __deg_x({0}), __deg_y({0}), coefs({}) {}
    Polynomial2D(const initializer_list2D<double> init);
    Polynomial2D(const Polynomial2D& other) = default;
    Polynomial2D(Polynomial2D&& other) = default;
    ~Polynomial2D() = default;

    Polynomial2D& operator=(const initializer_list2D<double> init);
    Polynomial2D& operator=(const Polynomial2D& other) = default;
    Polynomial2D& operator=(Polynomial2D&& other) = default;

    // Getters and setters

    inline size_t deg_x() const { return __deg_x.empty() ? 0 : *__deg_x.rbegin(); }
    inline size_t deg_y() const { return __deg_y.empty() ? 0 : *__deg_y.rbegin(); }

    // Get or set coefficient for x^i * y^j
    double get_coefficient(size_t exp_x, size_t exp_y) const;

    // Set coefficient for x^i * y^j
    void set_coefficient(size_t exp_x, size_t exp_y, double value);

    // Calculus

    Polynomial2D partial_diff_x() const;
    Polynomial2D partial_diff_y() const;

    // Evaluation

    double operator()(double x, double y) const;

    // Arithmetic

    Polynomial2D operator-() const;

    Polynomial2D& operator+=(const Polynomial2D& rhs);
    Polynomial2D& operator-=(const Polynomial2D& rhs);
    Polynomial2D& operator*=(double scalar);

    // Comparison

    inline bool operator==(const Polynomial2D& rhs) const {
        return *__deg_x.rbegin() == *rhs.__deg_x.rbegin() &&
               *__deg_y.rbegin() == *rhs.__deg_y.rbegin() && coefs == rhs.coefs;
    }

    inline bool operator!=(const Polynomial2D& rhs) const { return !(*this == rhs); }
};

// Output

std::ostream& operator<<(std::ostream& os, const Polynomial2D& poly);

std::string toString(const Polynomial2D& poly);

// Arithmetic

Polynomial2D operator+(const Polynomial2D& lhs, const Polynomial2D& rhs);
Polynomial2D operator-(const Polynomial2D& lhs, const Polynomial2D& rhs);
Polynomial2D operator*(const Polynomial2D& poly, double scalar);
inline Polynomial2D operator*(double scalar, const Polynomial2D& poly) {
    return poly * scalar;
}

#endif  // POLYNOMIAL2D_H
