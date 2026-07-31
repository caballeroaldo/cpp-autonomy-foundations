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

        // Debugging methods

        void printPrediction() const;
        void printCorrection() const;

        void printCovariance() const;

        void printInnovation(const Eigen::Vector2d& innovation) const;

        void printInnovationCovariance(const Eigen::Matrix2d& innovationCovariance) const;

        void printKalmanGain(const Eigen::Matrix<double, 4 , 2>& kalmanGain) const;
    
    private:
        Eigen::Vector4d state_;
        Eigen::Matrix4d covariance_;
        Eigen::Matrix4d processNoise_;
        Eigen::Matrix2d measurementNoise_;
        Eigen::Matrix<double, 2, 4> measurementMatrix_;

        bool isInitialized_;

        Eigen::Matrix4d transitionMatrix(double dt) const;
};

#endif