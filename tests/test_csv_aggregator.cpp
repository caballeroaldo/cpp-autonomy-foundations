#include <iostream>

#include "../csv_aggregator.hpp"

int main()
{
    try
    {
        aggregateBenchmarkReports("output");

        std::cout
            << "CSV aggregation completed successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "CSV aggregation failed:\n"
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}