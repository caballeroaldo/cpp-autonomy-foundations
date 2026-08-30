#include "csv_report_export.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

void exportBenchmarkReportCsv(const BenchmarkReport& report, const std::string& outputDirectory) {
    fs::create_directories(outputDirectory);

    std::ofstream outputFile(outputDirectory + "/benchmark_report.csv");

    if (!outputFile) {
        throw std::runtime_error("Unable to create benchmark_report.csv");
    }

    // Header
    outputFile
        << "Dataset,"
        << "Method,"
        << "IdentitySwitches,"
        << "Fragmentations,"
        << "AverageTrackContinuity,"
        << "IdentityPreservationRate,"
        << "TracksCreated,"
        << "TracksDeleted,"
        << "SuccessfulAssociations,"
        << "MissedAssociations,"
        << "AveragePredictionError,"
        << "MaximumPredictionError,"
        << "TotalAssociationRuntimeMs,"
        << "AverageAssociationRuntimeMs\n";
    
    // Experiment Data
    std::string method;

    switch(report.associationMethod) {
        case AssociationMethod::Greedy:
            method = "Greedy";
            break;
        case AssociationMethod::Hungarian:
            method = "Hungarian";
            break;
    }

    outputFile
        << report.datasetName << ","
        << method << ","
        << report.evaluation.identitySwitches << ","
        << report.evaluation.fragmentations << ","
        << report.evaluation.averageTrackContinuity << ","
        << report.evaluation.identityPreservationRate * 100.0 << ","
        << report.trackerMetrics.tracksCreated << ","
        << report.trackerMetrics.tracksDeleted << ","
        << report.trackerMetrics.successfulAssociations << ","
        << report.trackerMetrics.missedAssociations << ","
        << report.trackerMetrics.averagePredictionError() << ","
        << report.trackerMetrics.maxPredictionError << ","
        << report.totalAssociationRuntimeMs << ",";

    if (report.processedAssociationFrames > 0) {
        outputFile
            << report.totalAssociationRuntimeMs /
            static_cast<double>(
                report.processedAssociationFrames);
    }
    else {
        outputFile << 0.0;
    }

    outputFile << '\n';
}