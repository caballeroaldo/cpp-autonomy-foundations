#pragma once

#include <vector>
#include "types.hpp"
#include "association.hpp"

struct CostMatrix
{
    std::vector<std::vector<double>> costs;
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
