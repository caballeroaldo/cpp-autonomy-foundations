#include "hungarian.hpp"
#include "association.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {

    struct ZeroState {
        int row = -1;
        int col = -1;

        bool starred = false;
        bool primed = false;
    };

    struct CoverState {
        std::vector<bool> coveredRows;
        std::vector<bool> coveredColumns;
    };

    //--------------------------------------------------
    // Matrix Reduction
    //--------------------------------------------------

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

    //--------------------------------------------------
    // Zero Initialization
    //--------------------------------------------------

    std::vector<ZeroState> initializeStarredZeros(const CostMatrix& matrix) {
        constexpr double EPSILON = 1e-9;
        std::vector<ZeroState> assignments;

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
                    ZeroState zero;
                    zero.row = static_cast<int>(row);
                    zero.col = static_cast<int>(col);

                    zero.starred = true;

                    assignments.push_back(zero);

                    usedRows[row] = true;
                    usedCols[col] = true;

                    break;
                }
            }
        }
        return assignments;
    }

    //--------------------------------------------------
    // Cover Computation
    //--------------------------------------------------

    CoverState computeMinimumCover(const CostMatrix& matrix, const std::vector<ZeroState>& starredZeros) {
        CoverState cover;

        cover.coveredRows.assign(
            matrix.costs.size(),
            false);

        if (!matrix.costs.empty()) {
            cover.coveredColumns.assign(
                matrix.costs[0].size(),
                false);
        }

        for (const auto& zero : starredZeros) {
            if (zero.row >= 0) {
                cover.coveredRows[zero.row] = true;
            }

            if (zero.col >= 0) {
                cover.coveredColumns[zero.col] = true;
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

    //--------------------------------------------------
    // Matrix Adjustment
    //--------------------------------------------------

    double findMinimumUncoveredValue(const CostMatrix& matrix, const CoverState& cover) {
        double minimum = std::numeric_limits<double>::max();

        for (std::size_t row = 0; row < matrix.costs.size(); ++row) {
            if (cover.coveredRows[row]) {
                continue;
            }

            for (std::size_t col = 0; col < matrix.costs[row].size(); ++col) {
                if (cover.coveredColumns[col]) {
                    continue;
                }

                minimum = std::min(minimum, matrix.costs[row][col]);
            }
        }
        return minimum;
    }

    void adjustMatrix(CostMatrix& matrix, const CoverState& cover, double minimum) {
        for (std::size_t row = 0; row < matrix.costs.size(); ++row) {
            for (std::size_t col = 0; col < matrix.costs[row].size(); ++col) {
                bool rowCovered = cover.coveredRows[row];
                bool colCovered = cover.coveredColumns[col];

                if (!rowCovered && !colCovered) {
                    matrix.costs[row][col] -= minimum;
                }
                else if (rowCovered && colCovered) {
                    matrix.costs[row][col] += minimum;
                }
            }
        }
    }

    //--------------------------------------------------
    // Uncovered Zero Search
    //--------------------------------------------------

    bool findUncoveredZero(const CostMatrix& matrix, const CoverState& cover, ZeroState& zero) {
        constexpr double EPSILON = 1e-9;

        for (std::size_t row = 0; row < matrix.costs.size(); ++row) {
            if (cover.coveredRows[row])
                continue;

            for (std::size_t col = 0; col < matrix.costs[row].size(); ++col) {
                if (cover.coveredColumns[col])
                    continue;

                if (std::abs(matrix.costs[row][col]) < EPSILON) {
                    zero.row = static_cast<int>(row);
                    zero.col = static_cast<int>(col);

                    zero.primed = true;

                    return true;
                }
            }
        }

        return false;
    }



    //--------------------------------------------------
    // Debug Utilities
    //--------------------------------------------------

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
}

//--------------------------------------------------
// Cost Matrix
//--------------------------------------------------

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

//--------------------------------------------------
// Hungarian Solver
//--------------------------------------------------

std::vector<Association> hungarianAssignment(CostMatrix matrix) {
    printCostMatrix(matrix, "Original Cost Matrix");
    reduceRows(matrix);
    reduceColumns(matrix);
    printCostMatrix(matrix, "Reduced Cost Matrix");

    while(true) {
        auto starredZeros = initializeStarredZeros(matrix);
        auto cover = computeMinimumCover(matrix, starredZeros);

        if (hasCompleteCover(cover, matrix)) {
            std::vector<Association> associations;
            for (const auto& zero : starredZeros) {
                Association association;
                association.trackIndex = zero.row;
                association.detectionIndex = zero.col;

                association.squaredDistance = 0.0;
                associations.push_back(association);
            }
            return associations;
        }

        double minimum = findMinimumUncoveredValue(matrix, cover);

        adjustMatrix(matrix, cover, minimum);

        printCostMatrix(matrix, "Adjusted Matrix");
    }

    std::cout << "Hungarian algorithm did not converge.\n";
    return {};
}