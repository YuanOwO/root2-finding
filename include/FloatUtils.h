#include <algorithm>
#include <cmath>

const double EPS = 1e-6;

inline bool isZero(double a) {
    return std::abs(a) < EPS;
}

inline bool isNegative(double a) {
    return std::signbit(a);
}

inline bool isEqual(double a, double b) {
    return std::abs(a - b) < EPS;
}

inline bool isnotEqual(double a, double b) {
    return std::abs(a - b) > EPS;
}

inline bool isLessThan(double a, double b) {
    return a - b < -EPS;
}

inline bool isLessThanOrEqual(double a, double b) {
    return a - b < EPS;
}

inline bool isGreaterThan(double a, double b) {
    return a - b > EPS;
}

inline bool isGreaterThanOrEqual(double a, double b) {
    return a - b > -EPS;
}
