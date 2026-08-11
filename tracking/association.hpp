#pragma once

#include <vector>

#include "types.hpp"
#include "kdtree.hpp"

struct Association {
    int trackIndex = -1;
    int detectionIndex = -1;
    double squaredDistance = 0.0;
};

struct CostMatrix {
    std::vector<std::vector<double>> costs;
};

struct CoverState {
    std::vector<bool> coveredRows;
    std::vector<bool> coveredColumns;
};

CostMatrix buildCostMatrix(
    const std::vector<Point>& predictedPositions,
    const std::vector<Point>& detections
);

CoverState coverIndependentZeros(
    const CostMatrix& matrix,
    const std::vector<Association>& assignments
);

bool hasCompleteCover(
    const CoverState& cover,
    const CostMatrix& matrix
);

Association findBestAssociation(
    Node* kdTree,
    const Point& detection,
    const std::vector<bool>& tracksUsed
);

std::vector<Association> associateTracks(
    const CostMatrix& CostMatrix
);

void reduceRows(CostMatrix& matrix);

void reduceColumns(CostMatrix& matrix);

std::vector<Association> findIndependentZeros(
    const CostMatrix& matrix
);

double findMinimumUncoveredValue(
    const CostMatrix& matrix,
    const CoverState& cover);

void adjustMatrix(
    CostMatrix& matrix,
    const CoverState& cover,
    double minimum);

bool hasCompleteCover(...);

std::vector<Association> hungarianAssignment(
    CostMatrix matrix
);

void printCostMatrix(
    const CostMatrix& matrix,
    const std::string& title = "Association Cost Matrix"
);

void printCoverState(
    const CostMatrix& cover
);