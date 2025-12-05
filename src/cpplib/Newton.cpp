#include "Newton.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

#include "FloatUtils.h"
#include "Polynomial2D.h"

// #define DEBUG

// 傳入算好的偏微分，不必每次都計算

NewtonResult newtonMethod2D(const Polynomial2D& f, const Polynomial2D& fx, const Polynomial2D& fy,
                            const Polynomial2D& g, const Polynomial2D& gx, const Polynomial2D& gy,
                            double x0, double y0, bool return_history = false) {
#ifdef DEBUG
    std::cout << " #          x_n          y_n          err" << std::endl
              << "-----------------------------------------" << std::endl
              << " 0 " << std::fixed << std::setprecision(9);
    if (!std::signbit(x0)) std::cout << " ";
    std::cout << x0 << " ";
    if (!std::signbit(y0)) std::cout << " ";
    std::cout << y0 << std::endl;
#endif
    double err = 0, err0 = 0;  // 初始化誤差 (亂設一個值)
    double x = x0, y = y0;

    std::vector<HistoryType> history;

    history.push_back({x0, y0, INFINITY});

    int i = 0;
    do {
        // Jacobian Matrix
        // J = [[fx(x, y), fy(x, y)], = [[a, b],
        //      [gx(x, y), gy(x, y)]]    [c, d]]
        double a = fx(x, y), b = fy(x, y);
        double c = gx(x, y), d = gy(x, y);

        double zf = f(x, y), zg = g(x, y);

        // 先分別算分子、分母
        double h = -d * zf + b * zg;
        double k = c * zf - a * zg;
        double det = a * d - b * c;

        if (isLessThanOrEqual(std::abs(det), EPS)) {
            // det 太小或為 0，無法繼續計算 => 發散
            x = y = err = INFINITY;
            // x = std::copysign(err, h);
            // y = std::copysign(err, k);
            break;
        } else {
            h /= det, k /= det;
            err0 = err;  // 儲存前一次的誤差
            err = std::sqrt(h * h + k * k);
            x += h, y += k;  // 更新 x, y
        }

        if (return_history) history.push_back({x, y, err});
        i++;

#ifdef DEBUG
        std::cout << std::setw(2) << i << " ";
        if (!std::signbit(x)) std::cout << " ";
        std::cout << x << " ";
        if (!std::signbit(y)) std::cout << " ";
        std::cout << y << " ";
        if (!std::signbit(err)) std::cout << " ";
        std::cout << err << " ";

        std::cout << err - err0 << std::endl;
#endif

    } while (isGreaterThan(std::abs(err - err0), EPS) && isfinite(err));

    Point root = {x, y};

    return {i, root, history};
}

/**
 * @brief 二維牛頓法
 *
 * @param f f(x, y)
 * @param g g(x, y)
 * @param x0 初始值
 * @param y0 初始值
 * @param return_history 是否記錄歷史
 * @return IterResult (迭代次數, (根的 x, 根的 y))
 */
NewtonResult newtonMethod2D(const Polynomial2D& f, const Polynomial2D& g, double x0, double y0,
                            bool return_history = false) {
    // 偏微分
    Polynomial2D fx = f.partial_diff_x(), fy = f.partial_diff_y();
    Polynomial2D gx = g.partial_diff_x(), gy = g.partial_diff_y();
    return newtonMethod2D(f, fx, fy, g, gx, gy, x0, y0, return_history);
}
