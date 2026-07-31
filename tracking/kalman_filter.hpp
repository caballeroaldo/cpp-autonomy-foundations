#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

#include "types.hpp"
#include <Eigen/Dense>


class KalmanFilter {
    public:
        KalmanFilter();

        void initialize(const Point& intialPosition);

        void predict(double dt = 1.0);

        void update(const Point& measurement, double dt = 1.0);

        Point position() const;

        Point velocity() const;
    
    private:
        Eigen::Vector4d state_;
        bool isInitialized_;

        Eigen::Matrix4d transitionMatrix(double dt) const;
};

#endif