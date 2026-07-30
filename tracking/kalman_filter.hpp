#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

#include "types.hpp"

struct State {
    double x = 0.0;
    double y = 0.0;

    double vx = 0.0;
    double vy = 0.0;
};

class KalmanFilter {
    public:
        KalmanFilter();

        void initialize(const Point& intialPosition);

        void predict(double dt = 1.0);

        void update(const Point& measurement, double dt = 1.0);

        Point position() const;

        Point velocity() const;
    
    private:
        State state_;
        bool isInitialized_;
};

#endif