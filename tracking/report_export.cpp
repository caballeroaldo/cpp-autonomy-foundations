#include "report_export.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

void exportEvaluationReport(
    const BenchmarkReport& report,
    const std::string& outputDirectory)
{
    fs::create_directories(outputDirectory);

    std::ofstream outputFile(outputDirectory + "/metrics.md");

    if (!outputFile) {
        throw std::runtime_error("Unable to create metrics report.");
    }

    outputFile << "# Tracking Evaluation\n\n";

    outputFile
        << "## Dataset\n\n"
        << report.datasetName
        << " \n\n";

    outputFile << "## Association Method\n\n";

    switch(report.associationMethod) {
        case AssociationMethod::Greedy:
            outputFile << "Greedy\n\n";
            break;
        case AssociationMethod::Hungarian:
            outputFile << "Hungarian\n\n";
            break;
    }

    // Tracking Metrics 

    outputFile
        << "## Tracking Metrics\n\n";

    outputFile
        << "| Metric | Value |\n";

    outputFile
        << "|--------|------:|\n";

    outputFile
        << "| Identity Switches | "
        << report.evaluation.identitySwitches
        << " |\n";

    outputFile
        << "| Fragmentations | "
        << report.evaluation.fragmentations
        << " |\n";

    outputFile
        << "| Average Track Continuity | "
        << report.evaluation.averageTrackContinuity
        << " |\n";

    outputFile
        << "| Identity Preservation Rate | "
        << report.evaluation.identityPreservationRate * 100.0
        << "% |\n\n";

    // Tracker Metrics

    outputFile
        << "## Tracker Metrics\n\n";

    outputFile
        << "| Metric | Value |\n";

    outputFile
        << "|--------|------:|\n";

    outputFile
        << "| Tracks Created | "
        << report.trackerMetrics.tracksCreated
        << " |\n";

    outputFile
        << "| Tracks Deleted | "
        << report.trackerMetrics.tracksDeleted
        << " |\n";

    outputFile
        << "| Successful Associations | "
        << report.trackerMetrics.successfulAssociations
        << " |\n";

    outputFile
        << "| Missed Associations | "
        << report.trackerMetrics.missedAssociations
        << " |\n";

    outputFile
        << "| Average Prediction Error | "
        << report.trackerMetrics.averagePredictionError()
        << " |\n";

    outputFile
        << "| Maximum Prediction Error | "
        << report.trackerMetrics.maxPredictionError
        << " |\n\n";

    // Association Metrics

    outputFile
        << "## Association Metrics\n\n";

    outputFile
        << "| Metric | Value |\n";

    outputFile
        << "|--------|------:|\n";

    outputFile
        << "| Total Association Runtime (ms) | "
        << report.totalAssociationRuntimeMs
        << " |\n";

    outputFile
        << "| Average Association Runtime (ms) | "
        << (report.processedAssociationFrames > 0
                ? report.totalAssociationRuntimeMs /
                    static_cast<double>(report.processedAssociationFrames)
                : 0.0)
        << " |\n\n";

}