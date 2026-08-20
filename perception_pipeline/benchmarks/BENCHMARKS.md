# Perception Pipeline Benchmark Suite

This document describes the synthetic benchmark datasets used to evaluate the perception pipeline. Each scenario isolates a specific tracking challenge while keeping the remaining scene characteristics unchanged, enabling reproducible comparisons of motion models and data association algorithms.

| Dataset | Purpose | Status |
|----------|---------|--------|
| prediction_demo_clean | Baseline constant-velocity motion without measurement noise | ✅ |
| acceleration_demo | Tests robustness to increasing vehicle speed | ✅ |
| curved_demo | Tests robustness to gradual heading changes | ✅ |
| occlusion_demo | Temporary loss of measurements | ✅ |
| crossing_demo | Vehicles crossing paths | ✅ |
| false_detection_demo | Random false measurements | ✅ |
