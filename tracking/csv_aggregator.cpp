#include "csv_aggregator.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

std::vector<fs::path> findBenchmarkReports(const std::string& outputDirectory) {
    std::vector<fs::path> reports;
    for (const auto& entry : fs::recursive_directory_iterator(outputDirectory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().filename() == "benchmark_report.csv") {
            reports.push_back(entry.path());
        }
    }

    return reports;
}

void aggregateBenchmarkReports(const std::string& outputDirectory) {
    std::vector<fs::path> reports = findBenchmarkReports(outputDirectory);
    bool headerWritten = false;

    fs::create_directories(outputDirectory);

    std::ofstream outputFile(outputDirectory + "/benchmark_results.csv");

    if (!outputFile) {
        throw std::runtime_error("Unable to create benchmark_results.csv");
    }

    for (const auto& reportPath : reports) {
        std::ifstream input(reportPath);

        if(!input) {
            continue;
        }

        std::string line;

        // Read header
        if(!std::getline(input, line)) {
            continue;
        }

        if(!headerWritten) {
            outputFile << line << "\n";
            headerWritten = true;
        }

        // Read the experiment row
        if (std::getline(input, line)) {
            outputFile << line << "\n";
        }
    }



}