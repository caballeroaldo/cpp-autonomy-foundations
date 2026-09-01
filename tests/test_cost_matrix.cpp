#include <iostream>
#include <vector>

#include "../hungarian.hpp"
#include "../types.hpp"

bool testSquareMatrix()
{
    std::cout << "\n========================================\n";
    std::cout << "Square Matrix Test\n";
    std::cout << "========================================\n";

    std::vector<Point> tracks =
    {
        {0,0},
        {10,0},
        {20,0}
    };

    std::vector<Point> detections =
    {
        {0,0},
        {10,0},
        {20,0}
    };

    CostMatrix matrix =
        buildCostMatrix(
            tracks,
            detections);
    std::cout << "Rows: " << matrix.costs.size() << "\n";
    std::cout << "Cols: " << matrix.costs[0].size() << "\n";

    std::cout << "Original Tracks: "
            << matrix.originalTrackCount << "\n";

    std::cout << "Original Detections: "
            << matrix.originalDetectionCount << "\n";

    for (std::size_t i = 0; i < matrix.costs.size(); ++i) {
        for (std::size_t j = 0; j < matrix.costs[i].size(); ++j) {
            std::cout << matrix.costs[i][j] << " ";
        }

        std::cout << "\n";
    }

    bool passed = true;

    if (matrix.costs.size() != 3)
        passed = false;

    if (matrix.costs[0].size() != 3)
        passed = false;

    if (matrix.originalTrackCount != 3)
        passed = false;

    if (matrix.originalDetectionCount != 3)
        passed = false;

    std::cout
        << (passed ? "PASSED\n" : "FAILED\n");

    return passed;
}

bool testMoreDetections()
{
    std::cout << "\n========================================\n";
    std::cout << "3 Tracks / 5 Detections\n";
    std::cout << "========================================\n";

    std::vector<Point> tracks =
    {
        {0,0},
        {10,0},
        {20,0}
    };

    std::vector<Point> detections =
    {
        {0,0},
        {10,0},
        {20,0},
        {30,0},
        {40,0}
    };

    CostMatrix matrix =
        buildCostMatrix(
            tracks,
            detections);
    std::cout << "Rows: " << matrix.costs.size() << "\n";
    std::cout << "Cols: " << matrix.costs[0].size() << "\n";

    std::cout << "Original Tracks: "
            << matrix.originalTrackCount << "\n";

    std::cout << "Original Detections: "
            << matrix.originalDetectionCount << "\n";

    for (std::size_t i = 0; i < matrix.costs.size(); ++i) {
        for (std::size_t j = 0; j < matrix.costs[i].size(); ++j) {
            std::cout << matrix.costs[i][j] << " ";
        }

        std::cout << "\n";
    }

    bool passed = true;

    if (matrix.costs.size() != 5)
        passed = false;

    if (matrix.costs[0].size() != 5)
        passed = false;

    if (matrix.originalTrackCount != 3)
        passed = false;

    if (matrix.originalDetectionCount != 5)
        passed = false;

    constexpr double DUMMY_COST = 1e12;

    for (std::size_t row = 3;
         row < matrix.costs.size();
         ++row)
    {
        for (double value : matrix.costs[row])
        {
            if (value != DUMMY_COST)
                passed = false;
        }
    }

    std::cout
        << (passed ? "PASSED\n" : "FAILED\n");

    return passed;
}

bool testMoreTracks()
{
    std::cout << "\n========================================\n";
    std::cout << "5 Tracks / 3 Detections\n";
    std::cout << "========================================\n";

    std::vector<Point> tracks =
    {
        {0,0},
        {10,0},
        {20,0},
        {30,0},
        {40,0}
    };

    std::vector<Point> detections =
    {
        {0,0},
        {10,0},
        {20,0}
    };

    CostMatrix matrix =
        buildCostMatrix(
            tracks,
            detections);

    std::cout << "Rows: " << matrix.costs.size() << "\n";
    std::cout << "Cols: " << matrix.costs[0].size() << "\n";

    std::cout << "Original Tracks: "
            << matrix.originalTrackCount << "\n";

    std::cout << "Original Detections: "
            << matrix.originalDetectionCount << "\n";

    for (std::size_t i = 0; i < matrix.costs.size(); ++i) {
        for (std::size_t j = 0; j < matrix.costs[i].size(); ++j) {
            std::cout << matrix.costs[i][j] << " ";
        }

        std::cout << "\n";
    }

    bool passed = true;

    if (matrix.costs.size() != 5)
        passed = false;

    if (matrix.costs[0].size() != 5)
        passed = false;

    if (matrix.originalTrackCount != 5)
        passed = false;

    if (matrix.originalDetectionCount != 3)
        passed = false;

    constexpr double DUMMY_COST = 1e12;

    for (std::size_t row = 0;
         row < matrix.costs.size();
         ++row)
    {
        for (std::size_t col = 3;
             col < matrix.costs[row].size();
             ++col)
        {
            if (matrix.costs[row][col] != DUMMY_COST)
                passed = false;
        }
    }

    std::cout
        << (passed ? "PASSED\n" : "FAILED\n");

    return passed;
}

int main()
{
    int passed = 0;
    int total = 0;

    ++total;
    passed += testSquareMatrix();

    ++total;
    passed += testMoreDetections();

    ++total;
    passed += testMoreTracks();

    std::cout
        << "\n========================================\n";
    std::cout
        << "Cost Matrix Test Summary\n";
    std::cout
        << "========================================\n";

    std::cout
        << "Passed "
        << passed
        << " / "
        << total
        << " tests.\n";

    return (passed == total) ? 0 : 1;
}