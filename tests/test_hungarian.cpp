#include <iostream>
#include <string>
#include <vector>

#include "../hungarian.hpp"
#include "../association.hpp"

struct HungarianTestCase
{
    std::string name;
    CostMatrix matrix;
    std::vector<Association> expected;
};

bool runTest(const HungarianTestCase& test)
{
    std::cout << "\n========================================\n";
    std::cout << test.name << "\n";
    std::cout << "========================================\n";

    //auto result = hungarianAssignment(test.matrix);

    std::vector<Association> result;
    try {
        result = hungarianAssignment(test.matrix);
    } catch (const std::exception& e) {
        std::cout << "\nException:\n";
        std::cout << e.what() << "\n";
        return false;
    }

    if (test.expected.empty()) {
        std::cout << "\nReturned " << result.size() << " assignments\n";
        return true;
    }

    bool passed = true;

    if (result.size() != test.expected.size())
    {
        passed = false;
    }
    else
    {
        for (std::size_t i = 0; i < result.size(); ++i)
        {
            if (result[i].trackIndex != test.expected[i].trackIndex ||
                result[i].detectionIndex != test.expected[i].detectionIndex)
            {
                passed = false;
                break;
            }
        }
    }

    std::cout << "\nExpected:\n";

    for (const auto& assignment : test.expected)
    {
        std::cout
            << "Track "
            << assignment.trackIndex
            << " -> Detection "
            << assignment.detectionIndex
            << "\n";
    }

    std::cout << "\nResult:\n";

    for (const auto& assignment : result)
    {
        std::cout
            << "Track "
            << assignment.trackIndex
            << " -> Detection "
            << assignment.detectionIndex
            << "\n";
    }

    std::cout << "\n";

    if (passed)
        std::cout << "PASSED\n";
    else
        std::cout << "FAILED\n";

    return passed;
}

HungarianTestCase makeBasic3x3Test()
{
    HungarianTestCase test;

    test.name = "Basic 3x3";

    test.matrix.costs =
    {
        {4,1,3},
        {2,0,5},
        {3,2,2}
    };
    test.matrix.originalTrackCount = test.matrix.costs.size();

    test.matrix.originalDetectionCount = test.matrix.costs[0].size();

    test.expected =
    {
        {0,1},
        {1,0},
        {2,2}
    };

    return test;
}

HungarianTestCase makeIdentityTest()
{
    HungarianTestCase test;

    test.name = "Identity Matrix";

    test.matrix.costs =
    {
        {0,5,5},
        {5,0,5},
        {5,5,0}
    };

    test.matrix.originalTrackCount = test.matrix.costs.size();

    test.matrix.originalDetectionCount = test.matrix.costs[0].size();    

    test.expected =
    {
        {0,0},
        {1,1},
        {2,2}
    };

    return test;
}

HungarianTestCase makeDiagonalTest()
{
    HungarianTestCase test;

    test.name = "Diagonal Minimum";

    test.matrix.costs =
    {
        {1,2,3},
        {2,1,3},
        {3,2,1}
    };

    test.matrix.originalTrackCount = test.matrix.costs.size();

    test.matrix.originalDetectionCount = test.matrix.costs[0].size();

    test.expected =
    {
        {0,0},
        {1,1},
        {2,2}
    };

    return test;
}

HungarianTestCase makeBenchmark4x4Test()
{
    HungarianTestCase test;

    test.name = "Classic 4x4";

    test.matrix.costs =
    {
        {9,2,7,8},
        {6,4,3,7},
        {5,8,1,8},
        {7,6,9,4}
    };

    test.matrix.originalTrackCount = test.matrix.costs.size();

    test.matrix.originalDetectionCount = test.matrix.costs[0].size();

    test.expected =
    {
        {0,1},
        {1,0},
        {2,2},
        {3,3}
    };

    return test;
}

HungarianTestCase makeFalseDetectionRegressionTest() {
    HungarianTestCase test;

    test.name = "False Detection Regression";

    test.matrix.costs = {
        {12909.8, 18628.3, 12.7564, 13143.0, 6366.1, 26271.2, 44202.4},
        {37601.8, 22181.2, 7047.61, 26482.3, 10.5363, 22104.5, 22605.3},
        {41797.1, 80699.5, 24631.9, 12464.4, 21481.4, 6.24639, 11673.3},
        {85175.9, 88762.3, 43827.2, 44751.5, 23378.3, 11865.5, 0.409285},
        {10187.7, 62048.8, 12081.2, 13.8817, 24997.4, 11948.8, 42957.8},
        {167957.0, 122900.0, 96130.0, 116450.0, 54557.0, 60125.0, 18824.0},
        {1e12, 1e12, 1e12, 1e12, 1e12, 1e12, 1e12}
    };

    test.matrix.originalTrackCount = 6;
    test.matrix.originalDetectionCount = 7;

    return test;
}

int main()
{
    int passed = 0;
    int total = 0;
    
    ++total;
    passed += runTest(makeBasic3x3Test());

    ++total;
    passed += runTest(makeIdentityTest());

    ++total;
    passed += runTest(makeDiagonalTest());

    ++total;
    passed += runTest(makeBenchmark4x4Test());
    

    ++total;
    passed += runTest(makeFalseDetectionRegressionTest());

    std::cout
        << "\n========================================\n"
        << "Hungarian Assignment Test Summary\n"
        << "========================================\n";

    std::cout
        << "Passed "
        << passed
        << " / "
        << total
        << " tests.\n";

    return (passed == total) ? 0 : 1;
}