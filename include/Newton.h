#ifndef NEWTON_H
#define NEWTON_H

#include <tuple>
#include <utility>
#include <vector>

#include "Polynomial2D.h"

typedef std::pair<double, double> Point;
typedef std::tuple<double, double, double> HistoryType;

struct NewtonResult {
    int iterations;
    Point root;
    std::vector<HistoryType> history;
};

NewtonResult newtonMethod2D(const Polynomial2D& f, const Polynomial2D& fx, const Polynomial2D& fy,
                            const Polynomial2D& g, const Polynomial2D& gx, const Polynomial2D& gy,
                            double x0, double y0, bool return_history);

NewtonResult newtonMethod2D(const Polynomial2D& f, const Polynomial2D& g, double x0, double y0,
                            bool return_history);

#endif  // NEWTON_H
