#include <iostream>
#include <vector>

#include "kalman_filter.hpp"

int main()
{
    KalmanFilter filter;

    std::vector<Point> measurements =
    {
        {101, 99},
        {109, 101},
        {121, 98},
        {129, 100},
        {141, 102},
        {149, 99}
    };

    filter.initialize(measurements.front());

    for (std::size_t i = 1; i < measurements.size(); ++i)
    {
        std::cout << "\n=========================\n";
        std::cout << "Frame " << i << "\n";
        std::cout << "=========================\n";

        filter.predict();
        filter.printPrediction();

        filter.update(measurements[i]);
        filter.printCorrection();
        filter.printCovariance();
    }

    return 0;
}