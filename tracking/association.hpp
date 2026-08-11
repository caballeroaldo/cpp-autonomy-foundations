#pragma once

#include <vector>

#include "types.hpp"
#include "kdtree.hpp"

struct Association {
    int trackIndex = -1;
    int detectionIndex = -1;
    double squaredDistance = 0.0;
};

Association findBestAssociation(
    Node* kdTree,
    const Point& detection,
    const std::vector<bool>& tracksUsed
);