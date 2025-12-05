#include "Polynomial2D.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "FloatUtils.h"

Polynomial2D::Polynomial2D(const initializer_list2D<double> init)
    : __deg_x({0}), __deg_y({0}), coefs({}) {
    size_t i = 0, j = 0;

    for (const auto& row : init) {
        j = 0;
        for (const auto& val : row) {
            if (i >= MAX_DEG || j >= MAX_DEG)
                throw std::out_of_range("Initializer list exceeds max degree " +
                                        std::to_string(MAX_DEG));
            update(i, j, val);
            ++j;
        }
        ++i;
    }
}

Polynomial2D& Polynomial2D::operator=(const initializer_list2D<double> init) {
    Polynomial2D temp(init);
    *this = std::move(temp);
    return *this;
}

////////////////////////////////////////////////////////////////////////

// Getters and setters

inline void Polynomial2D::update(size_t exp_x, size_t exp_y, double value) {
    if (isZero(coefs[exp_x][exp_y]) && !isZero(value)) {
        __deg_x.insert(exp_x);
        __deg_y.insert(exp_y);
    } else if (!isZero(coefs[exp_x][exp_y]) && isZero(value)) {
        __deg_x.erase(__deg_x.find(exp_x));
        __deg_y.erase(__deg_y.find(exp_y));
    }

    coefs[exp_x][exp_y] = value;
}

double Polynomial2D::get_coefficient(size_t exp_x, size_t exp_y) const {
    if (exp_x >= MAX_DEG || exp_y >= MAX_DEG)
        throw std::out_of_range("Index out of range in get_coefficient the max degree is " +
                                std::to_string(MAX_DEG));

    return coefs[exp_x][exp_y];
}

void Polynomial2D::set_coefficient(size_t exp_x, size_t exp_y, double value) {
    if (exp_x >= MAX_DEG || exp_y >= MAX_DEG)
        throw std::out_of_range("Index out of range in set_coefficient the max degree is " +
                                std::to_string(MAX_DEG));

    update(exp_x, exp_y, value);
}

////////////////////////////////////////////////////////////////////////

// I/O

Polynomial2D string2poly(const std::string& str) {
    // TODO: not my implementation, may have bugs
    Polynomial2D poly;
    std::istringstream iss(str);
    std::string term;

    while (iss >> term) {
        // 解析每一項
        double coef = 1.0;
        size_t exp_x = 0, exp_y = 0;

        // 解析係數
        size_t pos = term.find('*');
        if (pos != std::string::npos) {
            coef = std::stod(term.substr(0, pos));
            term = term.substr(pos + 1);
        }

        // 解析變數
        pos = term.find('^');
        if (pos != std::string::npos) {
            if (term[0] == 'x') {
                exp_x = std::stoul(term.substr(pos + 1));
            } else if (term[0] == 'y') {
                exp_y = std::stoul(term.substr(pos + 1));
            }
        } else {
            if (term[0] == 'x') {
                exp_x = 1;
            } else if (term[0] == 'y') {
                exp_y = 1;
            }
        }

        poly.set_coefficient(exp_x, exp_y, coef);
    }

    return poly;
}

std::ostream& operator<<(std::ostream& os, const Polynomial2D& poly) {
    bool isFirstTerm = true;

    // 只有常數項
    if (poly.deg_x() == 0 && poly.deg_y() == 0) {
        os << poly.get_coefficient(0, 0);
        return os;
    }

    // 從高次到低次 (降冪)
    // size_t 是 unsigned，-1 == 0xFFFF...
    for (size_t i = poly.deg_x(); i <= poly.deg_x(); --i) {
        for (size_t j = poly.deg_y(); j <= poly.deg_y(); --j) {
            double c = poly.get_coefficient(i, j);
            if (isZero(c)) continue;

            // 非首項用加減號連接
            if (!isFirstTerm) {
                if (isNegative(c)) {
                    os << " - ";
                } else {
                    os << " + ";
                    c = -c;
                }
            }
            isFirstTerm = false;

            // 系數

            if (isEqual(c, -1.0)) {  // 係數為 -1 時只顯示負號 (只會出現在首項)
                os << "-";
            } else if (isnotEqual(c, 1.0) || (i == 0 && j == 0)) {  // 常數項或係數非 1 時顯示係數
                os << c;
                if (i > 0 || j > 0) os << "*";  // 後面有變數
            }

            // x 次方
            if (i > 0) {
                os << "x";
                if (i > 1) os << "^" << i;  // 1 次方時省略
                if (j > 0) os << "*";       // 後面有 y
            }

            // y 次方
            if (j > 0) {
                os << "y";
                if (j > 1) os << "^" << j;  // 1 次方時省略
            }
        }
    }

    return os;
}

std::string toString(const Polynomial2D& poly) {
    std::ostringstream oss;
    oss << poly;
    return oss.str();
}

////////////////////////////////////////////////////////////////////////

// Calculus

Polynomial2D Polynomial2D::partial_diff_x() const {
    Polynomial2D ret;

    // x^0 項微分後為 0，跳過 i = 0
    for (size_t i = 1; i <= deg_x(); ++i) {
        for (size_t j = 0; j <= deg_y(); ++j) {
            ret.update(i - 1, j, coefs[i][j] * i);
        }
    }

    return ret;
}

Polynomial2D Polynomial2D::partial_diff_y() const {
    Polynomial2D ret;

    // y^0 項微分後為 0，跳過 j = 0
    for (size_t i = 0; i <= deg_x(); ++i) {
        for (size_t j = 1; j <= deg_y(); ++j) {
            ret.update(i, j - 1, coefs[i][j] * j);
        }
    }
    return ret;
}

double Polynomial2D::operator()(double x, double y) const {
    std::vector<double> pow_x(MAX_DEG), pow_y(MAX_DEG);
    double ret = 0.0;

    pow_x[0] = pow_y[0] = 1.0;

    for (size_t j = 1; j <= deg_y(); ++j) {
        pow_y[j] = j & 1 ? pow_y[j - 1] * y : pow_y[j >> 1] * pow_y[j >> 1];
    }

    for (size_t i = 0; i <= deg_x(); ++i) {
        if (i) pow_x[i] = i & 1 ? pow_x[i - 1] * x : pow_x[i >> 1] * pow_x[i >> 1];

        for (size_t j = 0; j <= deg_y(); ++j)
            ret += coefs[i][j] * pow_x[i] * pow_y[j];
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////

// Arithmetic

Polynomial2D Polynomial2D::operator-() const {
    Polynomial2D ret;

    for (size_t i = 0; i <= deg_x(); ++i) {
        for (size_t j = 0; j <= deg_y(); ++j) {
            ret.update(i, j, -coefs[i][j]);
        }
    }

    return ret;
}

Polynomial2D& Polynomial2D::operator+=(const Polynomial2D& rhs) {
    size_t max_deg_x = std::max(deg_x(), rhs.deg_x()), max_deg_y = std::max(deg_y(), rhs.deg_y());

    for (size_t i = 0; i <= max_deg_x; ++i) {
        for (size_t j = 0; j <= max_deg_y; ++j) {
            update(i, j, coefs[i][j] + rhs.coefs[i][j]);
        }
    }

    return *this;
}

Polynomial2D& Polynomial2D::operator-=(const Polynomial2D& rhs) {
    size_t max_deg_x = std::max(deg_x(), rhs.deg_x()), max_deg_y = std::max(deg_y(), rhs.deg_y());

    for (size_t i = 0; i <= max_deg_x; ++i) {
        for (size_t j = 0; j <= max_deg_y; ++j) {
            update(i, j, coefs[i][j] - rhs.coefs[i][j]);
        }
    }

    return *this;
}

Polynomial2D& Polynomial2D::operator*=(double scalar) {
    for (size_t i = 0; i <= deg_x(); ++i) {
        for (size_t j = 0; j <= deg_y(); ++j) {
            coefs[i][j] *= scalar;
        }
    }

    return *this;
}

Polynomial2D operator+(const Polynomial2D& lhs, const Polynomial2D& rhs) {
    Polynomial2D ret(lhs);
    ret += rhs;
    return ret;
}

Polynomial2D operator-(const Polynomial2D& lhs, const Polynomial2D& rhs) {
    Polynomial2D ret(lhs);
    ret -= rhs;
    return ret;
}

Polynomial2D operator*(const Polynomial2D& poly, double scalar) {
    Polynomial2D ret(poly);
    ret *= scalar;
    return ret;
}
