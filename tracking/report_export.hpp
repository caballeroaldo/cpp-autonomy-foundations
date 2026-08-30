#ifndef REPORT_EXPORT_HPP
#define REPORT_EXPORT_HPP

#include <string>

#include "evaluation.hpp"
#include "metrics.hpp"
#include "association.hpp"

struct BenchmarkReport {
    std::string datasetName;
    AssociationMethod associationMethod;
    EvaluationReport evaluation;
    TrackerMetrics trackerMetrics;

    double totalAssociationRuntimeMs = 0.0;
    std::size_t processedAssociationFrames = 0;
};

void exportEvaluationReport(
    const BenchmarkReport& report,
    const std::string& outputDirectory);

#endif