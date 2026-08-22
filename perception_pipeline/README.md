# Perception Pipeline Foundations (C++)

This project implements a modular C++ perception pipeline for multi-object tracking. It builds on the standalone tracking library by integrating file-based detection ingestion, motion prediction, benchmark generation, quantitative evaluation, and visualization into a cohesive perception workflow.

The project demonstrates how detections flow through a perception pipeline:

→ Frame Detections
→ Motion Prediction
→ KD-Tree Data Association
→ Track Update
→ Evaluation
→ Visualization

Rather than focusing solely on tracking accuracy, the project emphasizes modular software architecture, reproducible benchmarking, and debugging tools commonly used in autonomous perception systems.

---

# Project Goals

The purpose of this project is to move from manual point entry to a more realistic data flow:

1. Load detections from frame files
2. Pass detections into the tracker
3. Maintain object identities across frames
4. Track trajectories over time
5. Evaluate tracking performance using quantitative metrics
6. Export tracking state for visualization and debugging
7. Build a modular tracking architecture supporting interchangable data association algorithms

This phase bridges the gap between a terminal-based tracker and a more complete perception system.

---

# Features

- File-based frame loading
- Automatic frame discovery
- Scenario-based benchmark generation
- Runtime-selectable data association
    - KD-tree Greedy
    - Hungarian optimal assignment
- Association benchmarking
- Association runtime evaluation
- Hungarian regression testing
- Constant-velocity Kalman Filter state estimation
- Tuned Kalman filter parameters
- Standalone Kalman filter validation
- Configurable tracking parameters
- Prediction error evaluation
- Persistent track identities
- Missed-frame handling
- Stale-track deletion
- Trajectory export (CSV)
- Frame state export (CSV)
- Trajectory visualization
- Tracker debug visualization

---

# Project Structure

```text
perception-pipeline/
├── benchmarks/
│   ├── BENCHMARKS.md
│   └── benchmark_results.md
│   
├── frames/
│   ├── prediction_demo_clean/
│   ├── prediction_demo_noisy/
│   ├── acceleration_demo/
│   ├── curved_demo/
│   ├── occlusion_demo/
│   ├── false_detection_demo/
│   └── crossing_demo/
│
├── output/
│   ├── track_0.csv
│   ├── track_1.csv
│   ├── frame_data.csv
│   ├── tracker_frame_10.png
│   └── trajectory_plot.png
│
├── frame_loader.hpp
├── frame_loader.cpp
├── trajectory_export.hpp
├── trajectory_export.cpp
├── frame_export.hpp
├── frame_export.cpp
├── generate_traffic_frames.py
├── visualize_tracks.py
├── main.cpp
└── README.md
```

---

# File Responsibilities

## `frame_loader.hpp / frame_loader.cpp`

Handles reading detection points from text files and converting them into `Point` objects.

## `frame_export.hpp / frame_export.cpp`

Exports tracker state for every successful association, including predicted positions, corrected positions, velocity estimates, and prediction errors. This data powers the tracker debug visualization.

## `main.cpp`

Coordinates the pipeline:

- Loads each frame
- Sends detections to the tracker
- Updates tracks
- Prints results

## `Tracking/` Module

Provides:

- `Track`
- `Observation`
- Greedy KD-tree matching
- Hungarian optimal assignment
- Runtime association selection
- Association benchmarking
- Trajectory history
- Track lifecycle management

---

# Frame Format

Each frame file contains one detection per line in the following format:

```text
x y
```

Example:

```text
100 200
300 400
500 500
```

---

# Synthetic Traffic Generator

The scenario-based benchmark generator now supports multiple configurable benchmark scenarios through a command-line interface. Each scenario isolates a specific tracking challenge while keeping the remaining scene characteristics unchanged, enabling reproducible evaluation of the perception pipeline.

For example:
```bash
python3 generate_traffic_frames.py \
    --scenario prediction \
    --output frames/prediction_demo_clean \
    --noise 0 \
    --clear
```
Supported benchmark scenarios:

• prediction
• acceleration
• curved
• occlusion
• false_detection
• crossing

---

# Pipeline Flow

The current perception pipeline follows the architecture below:

```text
Benchmark Scenario Generator
          ↓
Frame Files
          ↓
Frame Loader
          ↓
Kalman State Estimation
          ↓
Association Layer
   ├── Greedy (KD-tree)
   └── Hungarian
          ↓
Track Update
          ↓
Evaluation Metrics
          ↓
      ┌───────────────┐
      │               │
Trajectory Export   Frame Export
      │               │
      ▼               ▼
Trajectory Plot   Tracker Debug View
```

This structure separates detection input from tracking logic, which is an important step toward perception systems.

---

# Kalman Filter State Estimation

The tracker uses a constant-velocity Kalman filter to estimate each object's position and velocity over time.

Rather than directly updating tracks from incoming detections, the tracker follows a two-stage prediction/correction cycle:

1. Predict the next state using the constant-velocity motion model.
2. Associate detections using KD-tree nearest-neighbor search.
3. Correct the predicted state using the matched detection.
4. Record the corrected state estimate in the track history.

Unlike the previous deterministic predictor, the Kalman filter maintains both a state estimate and an uncertainty estimate (covariance), allowing the tracker to gradually improve its prediction accuracy as additional observations become available.

---

# Example Demo Run

## Input Files

### `frames/frame1.txt`

```text
100 200
300 400
```

### `frames/frame2.txt`

```text
105 205
298 398
700 700
```

### `frames/frame3.txt`

```text
110 210
705 705
```

---

## Example Output

```text
====================================
Frame 1
====================================
Initialized tracks from frames/frame1.txt

====================================
Frame 2
====================================
[Match] (105, 205) -> Track 0
[Match] (298, 398) -> Track 1
[New]   (700, 700) -> Track 2

Track 0 [missed=0]
  Path: F1(100,200) -> F2(105,205)

Track 1 [missed=0]
  Path: F1(300,400) -> F2(298,398)

Track 2 [missed=0]
  Path: F2(700,700)

====================================
Frame 3
====================================

[Match] (110, 210) -> Track 0
[Match] (705, 705) -> Track 2

Track 0 [missed=0]
  Path: F1(100,200) -> F2(105,205) -> F3(110,210)

Track 1 [missed=1]
  Path: F1(300,400) -> F2(298,398)

Track 2 [missed=0]
  Path: F2(700,700) -> F3(705,705)
```

---

# Visualization

Trajectory data is exported as CSV files and visualized using a Python plotting utility.

Generate the visualization for tracker plot at a specific frame and saved to a specifc output directory:

```bash
python3 visualize_tracks.py \
  --tracker \ 
  --frame 10 \ 
  --output output
```

```bash
python3 visualize_tracks.py \
    --trajectory \
    --output output
```

Example output:

![Synthetic Traffic Tracking Demo](output/trajectory_plot.png)

Each trajectory represents the Kalman Filter's corrected state estimate after incorporating each measurement. The resulting paths are smoother than the raw detections while preserving persistent object identities across frames.

Frame labels (`F1`, `F2`, etc.) indicate temporal progression and demonstrate persistent object identity across frames.

# Tracker Debug Visualization

The tracker exports frame-level debugging information that can be visualized independently of the trajectory plots.

The visualization displays:

- Predicted positions (×)
- Corrected track positions (●)
- Association lines
- Estimated velocity vectors
- Track identities

Example Output: 

![KD-Tree Tracker Debug View](output/tracker_frame_10.png)

---

# Benchmarking

The pipeline includes configurable synthetic datasets for evaluating prediction performance.
All benchmark datasets are generated through the scenario-based frame generator, allowing experiments to be reproduced using a single command-line argument.

Current benchmark scenarios include:

- Prediction (clean)
- Prediction (noisy)
- Acceleration
- Curved motion
- Temporary occlusion
- False detections
- Crossing vehicles

Prediction quality is evaluated using:

- Average prediction error
- Maximum prediction error
- Successful associations
- Missed associations

Association algorithms evaluated:

- Greedy KD-tree
- Hungarian optimal assignment

Metrics include:

- Prediction error
- Successful associations
- Track creation/deletion
- Association runtime


These benchmarks provide a baseline for evaluating future motion models such as Kalman filtering.

## Benchmark Documentation

The benchmark suite is documented separately:

- `benchmarks/BENCHMARKS.md` describes the purpose and design of each benchmark scenario.
- `benchmarks/BENCHMARK_RESULTS.md` records quantitative results and observations for each benchmark scenario.

Together, these benchmark scenarios provide a reproducible framework for evaluating future tracking improvements, including alternative motion models and data association algorithms.

---

# Kalman Filter Configuration

The Kalman filter parameters were empirically tuned using the synthetic benchmark datasets.

| Parameter | Value | Purpose |
|-----------|------:|---------|
| Initial Covariance (P₀) | 100 × I | High initial uncertainty allows rapid convergence during the first observations. |
| Process Noise (Q) | 0.1 × I | Models uncertainty in the constant-velocity motion model. |
| Measurement Noise (R) | 1.0 × I | Models uncertainty in the incoming detections. |

These values significantly improved prediction accuracy compared to the untuned baseline while maintaining identical association performance.

# Kalman Filter Evaluation

The tuned Kalman filter was evaluated using the synthetic benchmark datasets developed for this project.

| Configuration | Avg Prediction Error | Max Prediction Error | Successful Associations | Missed Associations |
|---------------|--------------------:|---------------------:|-------------------------:|--------------------:|
| Untuned Kalman Filter | 9.8859 | 24.4131 | 47 | 2 |
| Tuned Kalman Filter | **4.5203** | **21.2603** | 47 | 2 |

The tuned configuration reduced the average prediction error by approximately **54%** while preserving the same number of successful and missed associations.

---

# Why This Phase Matters

This project is the first step toward a larger perception pipeline because it introduces:

- Structured input ingestion
- Separation of detection from tracking
- Reusable system components
- Pipeline-style thinking

Instead of manually typing points into the terminal, the tracker now consumes detections from files, which is closer to how perception systems work in practice.

---

# Testing & Validation

The perception pipeline is verified through several complementary testing strategies.

## Unit Testing

The Hungarian assignment implementation is validated using deterministic cost matrices covering:

- Basic 3×3 assignment
- Identity assignment
- Diagonal minimum assignment
- Classic 4×4 benchmark
- False-detection regression case

## Regression Testing

A regression test derived from the false-detection benchmark ensures previously discovered assignment failures do not reappear.

## Scenario Validation

Both Greedy and Hungarian association algorithms are evaluated using the synthetic benchmark suite:

- Prediction
- Occlusion
- Crossing
- False Detection

These scenarios validate the complete tracking pipeline under progressively more challenging conditions.

## Quantitative Evaluation

The tracker records:

- Tracks created
- Tracks deleted
- Successful associations
- Prediction error
- Association runtime

# Current Limitations

The current implementation intentionally keeps the perception pipeline simple.

Known limitations:

- Frame data comes from synthetic detections rather than real sensors
- Uses a constant-velocity Kalman filter that assumes linear motion between observations
- No image or video processing yet
- No OpenCV integration yet
- No identity-switch evaluation yet
- No association gating
- Constant-velocity motion model
- Synthetic detections only

---

# Future Improvements

Motion Models

- Adaptive process and measurement noise
- Constant-acceleration Kalman filter

Data Association

- Identity-switch evaluation
- Mahalanobis gating
- Adaptive association thresholds

Benchmarking

- Dense multi-object intersections
- Closely spaced vehicles
- Stop-and-go traffic

Visualization

- Animated tracker visualization

Perception

- OpenCV integration
- Real sensor integration

---

# Learning Outcomes

This project strengthens:

- File I/O
- Data ingestion
- Modular C++ design
- Pipeline architecture
- Tracking system integration
- Systems thinking for perception-style software

---

# Relationship to the Tracker Project

This folder builds directly on the work done in the `tracking/` folder.

The `tracking/` module provides the tracking engine, while this folder focuses on how detections enter the system and flow through the pipeline.

Together, they form the foundation for a future perception-style application.

---

# Development Progress

## Completed
- File-based detection ingestion
- KD-tree accelerated association
- Hungarian assignment
- Runtime-selectable association algorithms
- Association benchmarking 
- Hungarian unit and regression testing
- Constant-velocity Kalman filter
- Kalman parameter tuning
- Standalone Kalman filter validation
- Prediction error evaluation
- Scenario-based benchmark generation framework
- Benchmark suite documentation
- Trajectory visualization
- Tracker debug visualization
- Visualization command-line interface
- Acceleration benchmark
- Curved-motion benchmark
- Occlusion benchmark
- False-detection benchmark
- Crossing benchmark

## In Progress
- Identity-switch evaluation

## Planned
- Benchmark Automation
- Mahalanobis gating
- Adaptive Kalman filter tuning
- Constant-acceleration motion model
- Animated visualization
- OpenCV integration
- Video-based perception