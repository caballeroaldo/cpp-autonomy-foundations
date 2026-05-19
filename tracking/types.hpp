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
};

struct Track {
    int id;
    Point position;
    int missedFrames;
    std::vector<Observation> history;
};

#endif