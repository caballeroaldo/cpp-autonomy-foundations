#pragma once

#include <vector>

#include "types.hpp"
#include "kdtree.hpp"

struct Association {
    int trackIndex;
    double squaredDistance;
};

Association findBestAssociation(
    Node* kdTree,
    const Point& detection,
    const std::vector<bool>& tracksUsed
);