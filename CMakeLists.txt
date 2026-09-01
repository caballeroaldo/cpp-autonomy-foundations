cmake_minimum_required(VERSION 3.20)
project(cpp_autonomy_foundations
    VERSION 1.0
    DESCRIPTION "A modular autonomous perception tracking and benchmarking platform"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(perception_pipeline
    perception_pipeline/main.cpp
    perception_pipeline/frame_loader.cpp
    perception_pipeline/trajectory_export.cpp
    perception_pipeline/frame_export.cpp

    tracking/kdtree.cpp
    tracking/tracker.cpp
    tracking/kalman_filter.cpp
    tracking/association.cpp
    tracking/hungarian.cpp
    tracking/metrics.cpp
    tracking/ground_truth_reader.cpp
    tracking/identity_evaluator.cpp
    tracking/evaluation.cpp
    tracking/report_export.cpp
    tracking/csv_report_export.cpp
    tracking/csv_aggregator.cpp
    tracking/benchmark_results_export.cpp
)

target_include_directories(perception_pipeline PRIVATE
    /opt/homebrew/opt/eigen/include/eigen3    
)


add_executable(benchmark_runner
    benchmark_runner.cpp
    tracking/csv_aggregator.cpp
    tracking/benchmark_results_export.cpp
)