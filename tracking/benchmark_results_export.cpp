#include "benchmark_results_export.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

void exportBenchmarkResultsMarkdown(const std::string& benchmarkResultsCsv, const std::string& outputFile) {
    fs::path outputPath(outputFile);

    fs::create_directories(outputPath.parent_path());

    std::ofstream markdown(outputFile);

    if (!markdown) {
        throw std::runtime_error(
            "Unable to create benchmark results markdown.");
    }

    markdown << "# Benchmark Results\n";

    std::ifstream inputFile(benchmarkResultsCsv);

    if(!inputFile) {
        throw std::runtime_error("Unable to open benchmark_results.csv");
    }

    std::string line;
    bool header = true;
    while (std::getline(inputFile, line)) {
       std::replace(line.begin(), line.end(), ',','|');
       markdown << "| " << line << " |\n";
       if (header) {
            int columns = static_cast<int>(std::count(line.begin(), line.end(), '|')) + 1;

            markdown << "|";

            for (int i = 0; i < columns; i++) {
                markdown << " --- |";
            }

            markdown << '\n';

            header = false;
        }
    }

}