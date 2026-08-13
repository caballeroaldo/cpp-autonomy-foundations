#pragma once

#include <vector>
#include "types.hpp"
#include "association.hpp"

struct CostMatrix
{
    std::vector<std::vector<double>> costs;

    std::size_t originalTrackCount = 0;
    std::size_t originalDetectionCount = 0;
};

//--------------------------------------------------
// Cost Matrix
//--------------------------------------------------

CostMatrix buildCostMatrix(
    const std::vector<Point>& predictedPositions,
    const std::vector<Point>& detections);

//--------------------------------------------------
// Hungarian Solver
//--------------------------------------------------

std::vector<Association> hungarianAssignment(
    CostMatrix matrix);
