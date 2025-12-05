#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "Newton.h"
#include "Polynomial2D.h"

using namespace std;

int main() {
    // f = -9 + y^2 + x^2
    Polynomial2D f = {
        {-9, 0, 1},
        {0,  0, 0},
        {1,  0, 0}
    };

    // g = -9 + y^2 - 3xy + x^2
    Polynomial2D g = {
        {-9, 0,  1},
        {0,  -3, 0},
        {1,  0,  0}
    };

    string cmd;
    Polynomial2D fx = f.partial_diff_x(), fy = f.partial_diff_y();
    Polynomial2D gx = g.partial_diff_x(), gy = g.partial_diff_y();

    // cout << fixed << setprecision(6);

    // 製作一個簡單的 CLI 介面
    cin >> cmd;
    if (cmd == "one") {
        double x0, y0;
        cin >> x0 >> y0;
        auto result = newtonMethod2D(f, fx, fy, g, gx, gy, x0, y0, true);
        for (const auto& [x, y, err] : result.history) {
            cout << x << " " << y << " " << err << endl;
        }
    } else if (cmd == "rect") {
        cin >> cmd;

        double x_start, x_end, y_start, y_end, step;
        cin >> x_start >> x_end >> y_start >> y_end >> step;

        for (double i = x_start; i <= x_end; i += step) {
            for (double j = y_start; j <= y_end; j += step) {
                auto result = newtonMethod2D(f, fx, fy, g, gx, gy, i, j, false);
                cout << i << " " << j << " ";
                if (cmd == "root")
                    cout << result.root.first << " " << result.root.second;
                else if (cmd == "iter")
                    cout << result.iterations;
                cout << endl;
            }
        }
    }
    return 0;
}
