#ifndef METRICS_HPP
#define METRICS_HPP

struct TrackerMetrics {
    // Tracking 
    int tracksCreated = 0;
    int tracksDeleted = 0;
    int successfulAssociations = 0;
    int missedAssociations = 0;

    // Prediction
    double totalPredictionError = 0.0;
    double maxPredictionError = 0.0;
    int predictionSamples = 0;

    double averagePredictionError() const {
        if (predictionSamples == 0) {
            return 0.0;
        }

        return totalPredictionError / predictionSamples;
    }

};

void printTrackerMetrics(const TrackerMetrics& metrics);

#endif