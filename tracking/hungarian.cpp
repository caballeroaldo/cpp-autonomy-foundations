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

    enum class Mark {
        None, 
        Star,
        Prime
    };

    struct CoverState {
        std::vector<bool> coveredRows;
        std::vector<bool> coveredColumns;
    };

    struct HungarianState {
        CostMatrix matrix;
        std::vector<ZeroState> starredZeros;
        std::vector<ZeroState> primedZeros;
        std::vector<std::vector<Mark>> marks;
        CoverState cover;
    };

    struct PathNode {
        int row;
        int col;
    };

    struct AugmentingPath {
        std::vector<ZeroState*> zeros;

        std::vector<PathNode> coordinates;
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

    std::vector<ZeroState> createInitialStarredZeros(const CostMatrix& matrix) {
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

    void initializeCover(HungarianState& state) {
        state.cover.coveredRows.assign(state.matrix.costs.size(), false);

        state.cover.coveredColumns.assign(state.matrix.costs[0].size(), false);

        for (const auto& zero : state.starredZeros) {
            state.cover.coveredColumns[zero.col] = true;
        }
    }

    bool allColumnsCovered(const HungarianState& state) {
        for (bool covered : state.cover.coveredColumns) {
            if (!covered) {
                return false;
            }
        }

        return true;
    }

    //--------------------------------------------------
    // Matrix Adjustment
    //--------------------------------------------------

    double findMinimumUncoveredValue(HungarianState& state) {
        double minimum = std::numeric_limits<double>::max();

        for (std::size_t row = 0; row < state.matrix.costs.size(); ++row) {
            if (state.cover.coveredRows[row]) {
                continue;
            }

            for (std::size_t col = 0; col < state.matrix.costs[row].size(); ++col) {
                if (state.cover.coveredColumns[col]) {
                    continue;
                }
                
                bool starred = false;

                for (const auto& current : state.starredZeros) {
                    if (current.row == static_cast<int>(row) && current.col == static_cast<int>(col)) {
                        starred = true;
                        break;
                    }
                }

                if (starred) {
                    continue;
                }

                bool primed = false;

                for (const auto& current : state.primedZeros) {
                    if (current.row == static_cast<int>(row) && current.col == static_cast<int>(col)) {
                        primed = true;
                        break;
                    }
                }

                if (primed) {
                    continue;
                }

                minimum = std::min(minimum, state.matrix.costs[row][col]);
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

    bool findUncoveredZero(const HungarianState& state, ZeroState& zero) {
        constexpr double EPSILON = 1e-9;

        for (std::size_t row = 0; row < state.matrix.costs.size(); ++row) {
            if (state.cover.coveredRows[row])
                continue;

            for (std::size_t col = 0; col < state.matrix.costs[row].size(); ++col) {
                if (state.cover.coveredColumns[col])
                    continue;

                if (std::abs(state.matrix.costs[row][col]) < EPSILON) {
                    bool starred = false;

                    for (const auto& current : state.starredZeros) {
                        if (current.row == static_cast<int>(row) && current.col == static_cast<int>(col)) {
                            starred = true;
                            break;
                        }
                    }

                    if (starred) {
                        continue;
                    }

                    bool primed = false;

                    for (const auto& current : state.primedZeros) {
                        if (current.row == static_cast<int>(row) && current.col == static_cast<int>(col)) {
                            primed = true;
                            break;
                        }
                    }

                    if (primed) {
                        continue;
                    }

                    zero.row = static_cast<int>(row);
                    zero.col = static_cast<int>(col);

                    zero.primed = true;

                    return true;
                }
            }
        }

        return false;
    }

    void primeZero(HungarianState& state, const ZeroState& zero) {
        ZeroState primed = zero;

        primed.starred = false;
        primed.primed = true;

        state.primedZeros.push_back(primed);
    }

    //--------------------------------------------------
    // Finding Zeros & Removing Prime Zeros and Covers
    //--------------------------------------------------
    const ZeroState* findStarInRow(const HungarianState& state, int row) {
        for (std::size_t col = 0; col < state.marks[row].size(); ++col) {
            if (state.marks[row][col] == Mark::Star) {
                static ZeroState zero;
                zero.row = row;
                zero.col = static_cast<int>(col);
                zero.starred = true;
                zero.primed = false;

                return &zero;
            }
        }

        return nullptr;
    }

    bool rowContainsStar(const HungarianState& state, int row) {

        return findStarInRow(state, row) != nullptr;
    }

    ZeroState* findMutablePrimeInRow(HungarianState& state, int row) {
        for (auto& zero : state.primedZeros) {
            if (zero.row == row) {
                return &zero;
            }
        }

        return nullptr;
    }

    ZeroState* findMutablePrime(HungarianState& state, int row, int col) {
        for (auto& zero : state.primedZeros) {
            if (zero.row == row && zero.col == col) {
                return &zero;
            }
        }
        return nullptr;
    }

    const ZeroState* findPrimeInRow(const HungarianState& state, int row) {
        for (std::size_t col = 0; col < state.marks[row].size(); ++col) {
            if (state.marks[row][col] == Mark::Prime) {
                static ZeroState zero;

                zero.row = row;
                zero.col = static_cast<int>(col);
                zero.starred = false;
                zero.primed = true;

                return &zero;
            }
        }

        return nullptr;
    }

    ZeroState* findMutableStarInColumn(HungarianState& state, int col) {
        for (auto& zero : state.starredZeros) {
            if (zero.col == col) {
                return &zero;
            }
        }
        return nullptr;
    }

    const ZeroState* findStarInColumn(const HungarianState& state, int col) {
        for (std::size_t row = 0; row < state.marks.size(); ++row) {
            if (state.marks[row][col] == Mark::Star) {
                static ZeroState zero;

                zero.row = static_cast<int>(row);
                zero.col = col;
                zero.starred = true;
                zero.primed = false;

                return &zero;
            }
        }

        return nullptr;
    }


    void clearPrimedZeros(HungarianState& state) {
        state.primedZeros.clear();
    }

    void clearCover(HungarianState& state) {
        std::fill(
            state.cover.coveredRows.begin(),
            state.cover.coveredRows.end(),
            false);

        std::fill(
            state.cover.coveredColumns.begin(),
            state.cover.coveredColumns.end(),
            false);
    }

    void updateCover(HungarianState& state, int row) {
        state.cover.coveredRows[row] = true;

        const ZeroState* star = findStarInRow(state, row);

        if (star != nullptr) {
            state.cover.coveredColumns[star->col] = false;
        }
    }

    //--------------------------------------------------
    // Augmenting Path
    //--------------------------------------------------

    AugmentingPath augmentPath(HungarianState& state, ZeroState* startingPrime) {
        AugmentingPath path;

        path.zeros.push_back(startingPrime);
        path.coordinates.push_back({startingPrime->row, startingPrime->col});

        while (true) {
            ZeroState* last = path.zeros.back();

            if (last->primed) {
                ZeroState* star = findMutableStarInColumn(state, last->col);

                if (star == nullptr) {
                    break;
                }

                path.zeros.push_back(star);
                path.coordinates.push_back({star->row, star->col});
            }
            else
            {
                ZeroState* prime = findMutablePrimeInRow(state, last->row);

                if (prime == nullptr) {
                    break;
                }
                path.zeros.push_back(prime);
                path.coordinates.push_back({prime->row, prime->col});
            }
        }

        return path;
    }

    void rebuildStarredZeros(HungarianState& state) {
        state.starredZeros.clear();

        for (std::size_t row = 0; row < state.marks.size(); ++row) {
            for (std::size_t col = 0; col < state.marks[row].size(); ++col) {
                if (state.marks[row][col] == Mark::Star) {
                    ZeroState zero;

                    zero.row = static_cast<int>(row);
                    zero.col = static_cast<int>(col);
                    zero.starred = true;
                    zero.primed = false;

                    state.starredZeros.push_back(zero);
                }
            }
        }
    }

    void applyAugmentingPath(HungarianState& state, const AugmentingPath& path) {
        /*
        std::cout << "\nApplying path:\n";

        for (const auto& node : path.coordinates) {
            std::cout << "(" << node.row << ", " << node.col << ")\n";
        }
        */

        // Remove starred zeros on the path.
        for (const auto& node : path.coordinates) {
            auto it = std::remove_if(state.starredZeros.begin(), state.starredZeros.end(),[&](const ZeroState& current) {
                return current.row == node.row && current.col == node.col;
            });
            state.starredZeros.erase(it, state.starredZeros.end());
        }

        // Promote primed zeros to starred zeros.
        for (const auto& node : path.coordinates) {
            auto prime = findMutablePrime(state, node.row, node.col);
            if (prime == nullptr) {
                continue;
            }

            ZeroState star = *prime;

            star.primed = false;
            star.starred = true;

            state.starredZeros.push_back(star);
        }

        // Remove all primed zeros.
        clearPrimedZeros(state);

        // Reset the cover.
        clearCover(state);

        // Reinitialize the cover.
        initializeCover(state);
        
        /*
        std::cout << "\nStarred zeros after augmentation\n";

        for (const auto& zero : state.starredZeros) {
            std::cout << "(" << zero.row << ", " << zero.col << ")\n";

        }
        std::cout << "\n";
        */
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

    void printAugmentingPath(const AugmentingPath& path) {
        std::cout << "\nAugmenting Path\n";
        std::cout << "----------------\n";
        
        std::cout << path.zeros.size() << "\n";

        for (const auto* zero : path.zeros) {
            std::cout
                << "("
                << zero->row
                << ", "
                << zero->col
                << ") ";

            if (zero->starred)
                std::cout << "[STAR]";

            if (zero->primed)
                std::cout << "[PRIME]";

            std::cout << "\n";
        }
    }
}

//--------------------------------------------------
// Cost Matrix
//--------------------------------------------------

CostMatrix buildCostMatrix(const std::vector<Point>& predictedPositions, const std::vector<Point>& detections) {
    CostMatrix matrix;

    std::size_t trackCount = predictedPositions.size();
    std::size_t detectionCount = detections.size();

    matrix.originalTrackCount = trackCount;
    matrix.originalDetectionCount = detectionCount;

    std::size_t dimension = std::max(trackCount, detectionCount);

    constexpr double DUMMY_COST = 1e12;

    matrix.costs.assign(dimension, std::vector<double>(dimension,DUMMY_COST));

    for (std::size_t i = 0; i < trackCount; i++) {
        for (std::size_t j = 0; j < detectionCount; j++) {
            matrix.costs[i][j] = squaredDistance(predictedPositions[i],detections[j]);
        }
    }

    return matrix;
}

//--------------------------------------------------
// Hungarian Solver
//--------------------------------------------------

std::vector<Association> hungarianAssignment(CostMatrix matrix) {
    HungarianState state;
    state.matrix = std::move(matrix);
    state.marks.assign(state.matrix.costs.size(), std::vector<Mark>(state.matrix.costs[0].size(), Mark::None));

    #ifdef HUNGARIAN_DEBUG
    printCostMatrix(state.matrix, "Original Matrix");
    #endif

    reduceRows(state.matrix);
    reduceColumns(state.matrix);

    #ifdef HUNGARIAN_DEBUG
    printCostMatrix(state.matrix, "Reduced Matrix");
    #endif
    
    state.starredZeros = createInitialStarredZeros(state.matrix);
    
    for(const auto& zero : state.starredZeros) {
        state.marks[zero.row][zero.col] = Mark::Star;
    }

    /*
    #ifdef HUNGARIAN_DEBUG
    std::cout << "\nInitial Mark Matrix\n";
    for (std::size_t row = 0; row < state.marks.size(); ++row) {
        for (std::size_t col = 0; col < state.marks[row].size(); ++col) {
            if (state.marks[row][col] == Mark::Star) {
                std::cout << "* ";
            }
            else {
                std::cout << ". ";
            }
        }

        std::cout << "\n";
    }
    #endif
    */

    initializeCover(state);

    int iteration = 0;

    

    while(true) {
        std::cout << "Iteration " << iteration << "\n";
        if (++iteration > 1000) {
            throw std::runtime_error("Hungarian algorithm exceeded iteration limit");
        }

        std::size_t coveredCount = 0;

        for (bool covered : state.cover.coveredColumns) {
            if (covered) {
                coveredCount++;
            }
        }

        std::cout << "Covered columns: " << coveredCount << " / " << state.cover.coveredColumns.size() <<  "\n";

        #ifdef HUNGARIAN_DEBUG
        printCoverState(state.cover);

        std::cout << "Columns covered: "
                  << allColumnsCovered(state)
                  << "\n";
        #endif


        if (allColumnsCovered(state)) {
            if (state.starredZeros.size() != state.matrix.costs.size()) {
                std::cout << "\n===== Hungarian Failure =====\n";
                std::cout << "Original Tracks: "
                        << state.matrix.originalTrackCount << "\n";

                std::cout << "Original Detections: "
                        << state.matrix.originalDetectionCount << "\n";

                std::cout << "Matrix Dimension: "
                        << state.matrix.costs.size() << "\n";

                std::cout << "Starred Zeros: "
                        << state.starredZeros.size() << "\n";
                std::cout << "\nStarred Zeros:\n";

                for (const auto& zero : state.starredZeros) {
                    std::cout
                        << "("
                        << zero.row
                        << ", "
                        << zero.col
                        << ")\n";
                }
                throw std::runtime_error ("Hungarian assignment incomplete.\n");
            }
            std::vector<Association> associations;
            for (const auto& zero : state.starredZeros) {
                if (static_cast<std::size_t>(zero.row) >= state.matrix.originalTrackCount) {
                    continue;
                }

                if (static_cast<std::size_t>(zero.col) >= state.matrix.originalDetectionCount) {
                    continue;
                }

                Association association;
                association.trackIndex = zero.row;
                association.detectionIndex = zero.col;

                association.squaredDistance = 0.0;
                associations.push_back(association);
            }
            return associations;
        }

        ZeroState uncoveredZero;

        bool foundUncovered = findUncoveredZero(state, uncoveredZero);
        
        bool starred = false;

        for (const auto& zero : state.starredZeros) {
            if (zero.row == uncoveredZero.row && zero.col == uncoveredZero.col) {
                starred = true;
                break;
            }
        }

        bool primed = false;

        for (const auto& zero : state.primedZeros) {
            if (zero.row == uncoveredZero.row && zero.col == uncoveredZero.col) {
                primed = true;
                break;
            }
        }
        /*
        if (foundUncovered) {
            std::cout
                << "Candidate status: "
                << "starred=" << starred
                << " primed=" << primed
                << "\n";
        }
        std::cout << "Found uncovered zero: " << (foundUncovered ? "Yes" : "No") << "\n";
        */

        if (foundUncovered) {
            // std::cout << "Zero at (" << uncoveredZero.row << ", " << uncoveredZero.col << ")\n";
            primeZero(state, uncoveredZero);
            // std::cout << "Primed zero\n";

            // std::cout << "Checking for starred zero in row" << uncoveredZero.row << "\n";
            if (rowContainsStar(state, uncoveredZero.row)){
                // std::cout << "Row contains a starred zero\n";
                updateCover(state, uncoveredZero.row);
                #ifdef HUNGARIAN_DEBUG
                printCoverState(state.cover);
                #endif
            }
            else {
                std::cout << "Building augmenting path\n";
                ZeroState* prime = &state.primedZeros.back();
                AugmentingPath path = augmentPath(state, prime);
                
                #ifdef HUNGARIAN_DEBUG
                printAugmentingPath(path);
                #endif

                applyAugmentingPath(state, path);
            }
        } else {
            double minimum = findMinimumUncoveredValue(state);
            std::cout << "Adjusting matrix by " << minimum << "\n";
            adjustMatrix(state.matrix, state.cover, minimum);
            
            #ifdef HUNGARIAN_DEBUG
            printCostMatrix(state.matrix, "Adjusted Matrix");
            #endif
        }
    }

    return {};
}