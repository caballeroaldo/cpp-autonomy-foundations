#include "metrics.hpp"
#include <iostream>

using namespace std;

void printTrackerMetrics(const TrackerMetrics& metrics) {
    cout << "\n====================\n";
    cout << "Tracker Metrics\n";
    cout << "======================\n";

    cout << "Tracks Created: " << metrics.tracksCreated << "\n";
    cout << "Tracks Deleted: " << metrics.tracksDeleted << "\n";

    cout << "Successful Associations: " << metrics.successfulAssociations << "\n";

    cout << "Missed Associations: " << metrics.missedAssociations << "\n";

    cout << "Average Prediction Error: " << metrics.averagePredictionError() << "\n";

    cout << "Maxiumum Prediction Error: " << metrics.maxPredictionError << "\n";

}