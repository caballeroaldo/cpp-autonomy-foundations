#ifndef TYPES_HPP
#define TYPES_HPP
#include <vector>

struct Point {
    double x;
    double y;
};

struct Observation {
    int frameNumber;
    Point position;
    Point predictedPosition;
    double predictionError = 0.0;
    
};

struct Track {
    int id;
    Point position;
    Point predictedPosition;
    Point velocity;
    int missedFrames;
    std::vector<Observation> history;
};

#endif