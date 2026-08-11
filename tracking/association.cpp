#include "association.hpp"
#include "tracker.hpp"
#include <limits>
#include <iomanip>
#include <iostream>
#include <cmath>

CostMatrix buildCostMatrix(const std::vector<Point>& predictedPositions, const std::vector<Point>& detections) {
    CostMatrix matrix;

    matrix.costs.resize(predictedPositions.size());

    for (std::size_t i = 0; i < predictedPositions.size(); i++) {
        matrix.costs[i].resize(detections.size());

        for (std::size_t j = 0; j < detections.size(); j++) {
            matrix.costs[i][j] = squaredDistance(predictedPositions[i],detections[j]);
        }
    }

    return matrix;
}


CoverState coverIndependentZeros(const CostMatrix& matrix, const std::vector<Association>& assignments) {
    CoverState cover;

    cover.coveredRows.assign(
        matrix.costs.size(),
        false);

    if (!matrix.costs.empty()) {
        cover.coveredColumns.assign(
            matrix.costs[0].size(),
            false);
    }

    for (const auto& assignment : assignments) {
        if (assignment.trackIndex >= 0) {
            cover.coveredRows[assignment.trackIndex] = true;
        }

        if (assignment.detectionIndex >= 0) {
            cover.coveredColumns[assignment.detectionIndex] = true;
        }
    }
    return cover;
}

bool hasCompleteCover(const CoverState& cover, const CostMatrix& matrix) {
    std::size_t lines = 0;

    for (bool row : cover.coveredRows) {
        if (row)
            lines++;
    }

    for (bool column : cover.coveredColumns) {
        if (column)
            lines++;
    }

    return lines >= matrix.costs.size();
}


Association findBestAssociation(Node* kdTree, const Point& detection, const std::vector<bool>& trackUsed) {
    Association result;
    result.trackIndex = -1;
    result.squaredDistance = std::numeric_limits<double>::max();

    if (kdTree == nullptr) {
        return result;
    }

    result.trackIndex = findBestUnusedTrackIndex(kdTree, detection, trackUsed);

    return result;

}

std::vector<Association> associateTracks(
    const CostMatrix& costMatrix)
{
    std::vector<Association> associations;

    if (costMatrix.costs.empty())
    {
        return associations;
    }

    std::vector<bool> trackUsed(costMatrix.costs.size(), false);
    std::vector<bool> detectionUsed(costMatrix.costs[0].size(), false);

    while (true)
    {
        double bestCost = std::numeric_limits<double>::max();

        int bestTrack = -1;
        int bestDetection = -1;

        for (std::size_t i = 0; i < costMatrix.costs.size(); ++i)
        {
            if (trackUsed[i])
                continue;

            for (std::size_t j = 0; j < costMatrix.costs[i].size(); ++j)
            {
                if (detectionUsed[j])
                    continue;

                if (costMatrix.costs[i][j] < bestCost)
                {
                    bestCost = costMatrix.costs[i][j];
                    bestTrack = static_cast<int>(i);
                    bestDetection = static_cast<int>(j);
                }
            }
        }

        if (bestTrack == -1)
        {
            break;
        }

        Association association;
        association.trackIndex = bestTrack;
        association.detectionIndex = bestDetection;
        association.squaredDistance = bestCost;

        associations.push_back(association);

        trackUsed[bestTrack] = true;
        detectionUsed[bestDetection] = true;
    }

    return associations;
}

void reduceRows(CostMatrix& matrix) {
    for (auto& row : matrix.costs) {
        if (row.empty())
            continue;

        double minimum = *std::min_element(
            row.begin(),
            row.end());

        for (double& value : row) {
            value -= minimum;
        }
    }
}

void reduceColumns(CostMatrix& matrix) {
    if (matrix.costs.empty())
        return;

    std::size_t rows = matrix.costs.size();
    std::size_t cols = matrix.costs[0].size();

    for (std::size_t col = 0; col < cols; ++col) {
        double minimum = matrix.costs[0][col];

        for (std::size_t row = 1; row < rows; ++row) {
            minimum = std::min(
                minimum,
                matrix.costs[row][col]);
        }

        for (std::size_t row = 0; row < rows; ++row) {
            matrix.costs[row][col] -= minimum;
        }
    }
}

std::vector<Association> findIndependentZeros(const CostMatrix& matrix) {
    constexpr double EPSILON = 1e-9;
    std::vector<Association> assignments;

    if (matrix.costs.empty()) {
        return assignments;
    }

    std::vector<bool> usedRows(matrix.costs.size(), false);
    std::vector<bool> usedCols(matrix.costs[0].size(), false);

    for (std::size_t row = 0; row < matrix.costs.size(); ++row) {
        for (std::size_t col = 0; col < matrix.costs[row].size(); ++col) {
            if (usedRows[row] || usedCols[col]) {
                continue;
            }

            if (std::abs(matrix.costs[row][col]) < EPSILON) {
                Association association;

                association.trackIndex =
                    static_cast<int>(row);

                association.detectionIndex =
                    static_cast<int>(col);

                association.squaredDistance = 0.0;

                assignments.push_back(association);

                usedRows[row] = true;
                usedCols[col] = true;

                break;
            }
        }
    }
    return assignments;
}

void printCostMatrix(const CostMatrix& matrix, const std::string& title)
{
    std::cout << "\n==============================\n";
    std::cout << title << "\n";
    std::cout << "==============================\n";

    if (matrix.costs.empty())
    {
        std::cout << "(empty)\n";
        return;
    }

    // Column headers (detections)
    std::cout << std::setw(8) << "";

    for (std::size_t j = 0; j < matrix.costs[0].size(); ++j)
    {
        std::cout << std::setw(10) << ("D" + std::to_string(j));
    }

    std::cout << "\n";

    // Rows (tracks)
    for (std::size_t i = 0; i < matrix.costs.size(); ++i)
    {
        std::cout << std::setw(8)
                  << ("T" + std::to_string(i));

        for (double cost : matrix.costs[i])
        {
            std::cout << std::setw(10)
                      << std::fixed
                      << std::setprecision(2)
                      << cost;
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

void printCoverState(const CoverState& cover) {
    std::cout << "\nCover State\n";
    std::cout << "-----------\n";

    std::cout << "\nCovered Rows\n";

    for (std::size_t i = 0; i < cover.coveredRows.size(); ++i) {
        std::cout
            << "T"
            << i
            << ": "
            << cover.coveredRows[i]
            << "\n";
    }

    std::cout << "\nCovered Columns\n";

    for (std::size_t j = 0; j < cover.coveredColumns.size(); ++j) {
        std::cout
            << "D"
            << j
            << ": "
            << cover.coveredColumns[j]
            << "\n";
    }
}


std::vector<Association> hungarianAssignment(CostMatrix matrix) {
    printCostMatrix(matrix, "Original Cost Matrix");
    reduceRows(matrix);
    reduceColumns(matrix);
    printCostMatrix(matrix, "Reduced Cost Matrix");

    std::vector<Association> assignments = findIndependentZeros(matrix);

    std::cout << "\nIndependent Zeros\n";
    std::cout << "-----------------\n";

    for (const auto& assignment : assignments) {
        std::cout  
            << "Track "
            << assignment.trackIndex
            <<" -> Detection "
            << assignment.detectionIndex
            << "\n";
    }

    CoverState cover = coverIndependentZeros(matrix, assignments);

    printCoverState(cover);

    if (hasCompleteCover(cover,matrix)) {
        std::cout << "\nComplete assignment found.\n";
    }
    else {
        std::cout << "\nIncomplete assignment\n";
    }

    return assignments;
}