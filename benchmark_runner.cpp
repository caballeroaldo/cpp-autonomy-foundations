#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>

#include "tracking/csv_aggregator.hpp"
#include "tracking/benchmark_results_export.hpp"
#include "tracking/config.hpp"

struct BenchmarkDataset {
    std::string datasetName;

    std::string datasetPath;
};

struct BenchmarkExperiment {
    std::string datasetName;

    std::string datasetPath;

    AssociationMethod associationMethod;
};

std::string buildCommand(const BenchmarkExperiment& experiment) {
    std::string method;
    switch (experiment.associationMethod)
    {
        case AssociationMethod::Greedy:
            method = "greedy";
            break;

        case AssociationMethod::Hungarian:
            method = "hungarian";
            break;
    }

    std::string outputDirectory = "output/" + experiment.datasetName + "/" + method;

    return "./bin/perception_pipeline "
        + experiment.datasetPath
        + " "
        + method
        + " "
        + outputDirectory;

}

bool runExperiment(const BenchmarkExperiment& experiment) {
    std::string command = buildCommand(experiment);

    std::cout << "Command:\n" << command << "\n\n";

    std::cout << "Dataset: " << experiment.datasetName << "\n";
    std::cout << "Association Method: ";
    switch(experiment.associationMethod) {
        case AssociationMethod::Greedy:
            std::cout << "Greedy\n";
            break;
        case AssociationMethod::Hungarian:
            std::cout << "Hungarian\n";
            break;
    } 
    
    std::cout << "\n";
    
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Experiment failed.\n";
        return false;
    }
    std::cout << "Experiment completed successfully.\n\n";
    return true;
}

int main() {

    std::vector<BenchmarkDataset> datasets;
    datasets.push_back({
        "prediction_demo_clean",
        "perception_pipeline/frames/prediction_demo_clean"});

    datasets.push_back({
        "prediction_demo_noisy",
        "perception_pipeline/frames/prediction_demo_noisy"});

    datasets.push_back({
        "acceleration_demo",
        "perception_pipeline/frames/acceleration_demo"});

    datasets.push_back({
        "curved_demo",
        "perception_pipeline/frames/curved_demo"});

    datasets.push_back({
        "occlusion_demo",
        "perception_pipeline/frames/occlusion_demo"});

    datasets.push_back({
        "crossing_demo",
        "perception_pipeline/frames/crossing_demo"});

    datasets.push_back({
        "false_detection_demo",
        "perception_pipeline/frames/false_detection_demo"});

    int successfulExperiments = 0;
    int failedExperiments = 0;
    std::vector<BenchmarkExperiment> failedExperimentsNames;

    for (const auto& dataset : datasets) {
        BenchmarkExperiment greedyExperiment;

        greedyExperiment.datasetName = dataset.datasetName;

        greedyExperiment.datasetPath = dataset.datasetPath;

        greedyExperiment.associationMethod = AssociationMethod::Greedy;

        if(runExperiment(greedyExperiment)) {
            ++successfulExperiments;
        } else {
            ++failedExperiments;
            failedExperimentsNames.push_back(greedyExperiment);
        }

        BenchmarkExperiment hungarianExperiment;

        hungarianExperiment.datasetName = dataset.datasetName;

        hungarianExperiment.datasetPath = dataset.datasetPath;

        hungarianExperiment.associationMethod = AssociationMethod::Hungarian;

        if(runExperiment(hungarianExperiment)) {
            ++successfulExperiments;
        } else {
            ++failedExperiments;
            failedExperimentsNames.push_back(hungarianExperiment);
        }
    }

    aggregateBenchmarkReports("output");

    exportBenchmarkResultsMarkdown("output/benchmark_results.csv", "docs/BENCHMARK_RESULTS.md");

    std::cout
        << "=====================================\n";

    std::cout
        << "Benchmark Summary\n";

    std::cout
        << "=====================================\n";

    std::cout
        << "Successful Experiments: "
        << successfulExperiments
        << '\n';

    std::cout
        << "Failed Experiments: "
        << failedExperiments
        << '\n';

    if (!failedExperimentsNames.empty()) {
        std::cout << "\nFailed Experiments\n";
        std::cout << "------------------\n";

        for (const auto& experiment : failedExperimentsNames) {
            std::cout << experiment.datasetName << " - ";

            switch (experiment.associationMethod) {
                case AssociationMethod::Greedy:
                    std::cout << "Greedy";
                    break;

                case AssociationMethod::Hungarian:
                    std::cout << "Hungarian";
                    break;
            }

            std::cout << '\n';
        }
    }
    return 0;
}