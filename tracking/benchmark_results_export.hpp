#ifndef BENCHMARK_RESULTS_EXPORT_HPP
#define BENCHMARK_RESULTS_EXPORT_HPP

#include <string>

void exportBenchmarkResultsMarkdown(
    const std::string& benchmarkResultsCsv,
    const std::string& outputFile);

#endif