#ifndef GROUND_TRUTH_HPP
#define GROUND_TRUTH_HPP

#include <vector>

#include "types.hpp"

struct GroundTruthObservation {
    int frameNumber;
    Point position;
};

struct GroundTruthObject {
    int objectId;

    std::vector<GroundTruthObservation> history;
};

#endif