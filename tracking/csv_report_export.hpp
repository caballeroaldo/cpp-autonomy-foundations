#ifndef CSV_REPORT_EXPORT_HPP
#define CSV_REPORT_EXPORT_HPP

#include <string>

#include "report_export.hpp"

void exportBenchmarkReportCsv(
    const BenchmarkReport& report,
    const std::string& outputDirectory);

#endif