# C++ Practice & Autonomous Systems Preparation

This repository contains my C++ projects and systems-focused engineering work as I build toward developing software for autonomous driving, perception, robotics, and real-time systems.

In addition to implementing tracking algorithms from first principles, this repository emphasizes reproducible evaluation through a configurable benchmark suite, allowing tracking approaches to be compared across standardized perception scenarios.

This repository follows a milestone-based development approach where each project builds upon the previous one to gradually construct the foundations of an autonomous perception stack.

Rather than relying on existing perception frameworks, each component is implemented from first principles to develop a deeper understanding of the algorithms, data structures, software architecture, and evaluation techniques used in modern perception systems.

The goal of this repository is to strengthen:

- Core C++ programming skills
- Data structures and algorithms
- Systems thinking
- Spatial reasoning and tracking
- Foundations for autonomous perception systems

---

## Highlights

- Modular C++ multi-object tracking system
- Modular data association framework
- KD-tree accelerated greedy association
- Hungarian optimal assignment
- Runtime-selectable association algorithms
- Constant-velocity Kalman Filter state estimation
- Synthetic benchmark generation and quantitative evaluation
- Quantitative prediction error evaluation
- Trajectory visualization
- Interactive tracker debugging and visualization tools
- Reproducible perception benchmark suite

---

# 📁 Repository Structure

```text
cpp-autonomy-foundations/
├── basics/
├── data_structures/
├── tracking/
├── perception_pipeline/
└── README.md
```

---

## Basics/

Introductory programs focused on core C++ concepts:

- Input/output (`cin`, `cout`)
- Control flow (`if`, `switch`, loops)
- Input validation
- Basic problem solving

### Current Programs

- `hello.cpp` – user input and output
- `calculator.cpp` – basic arithmetic with error handling
- `even-odd.cpp` – determining if user input is an even or odd integer
- `vector-average.cpp` – finding the average of a collection of numbers using vectors
- `max-number.cpp` – finding the maximum value in a collection

---

## DataStructures/

Programs focused on building fundamental data structures and algorithms.

### Current Programs

- `distance-two-points.cpp` – computes the distance between two `(x,y)` coordinates
- `nearest-point.cpp` – returns the nearest point in a collection relative to a queried point
- `kdtree.cpp` – implements a 2D KD-Tree using recursive spatial partitioning and nearest-neighbor search

### Key Concepts

- Recursive tree construction
- Spatial partitioning
- Nearest-neighbor search
- Search pruning
- Algorithm optimization

---

## Tracking/

Projects focused on object tracking and state management across frames.

### Multi-Frame Object Tracker ⭐

A modular C++ tracking system that maintains object identities across sequential frames using:

- KD-tree accelerated data association
- Frame-aware observations
- Persistent track identities
- Trajectory history
- Missed-frame handling
- Automatic stale-track deletion

### Key Concepts

- Multi-frame tracking
- State management over time
- Object lifecycle management
- KD-tree integration
- Real-time systems thinking

See: `tracking/README.md`

---

## PerceptionPipeline/

Projects focused on moving from manually entered detections toward perception-style data pipelines.

The perception pipeline includes a configurable benchmark generation framework that produces reproducible synthetic datasets for evaluating motion models, track management, and data association.

### Current Features

- File-based frame ingestion
- Automatic frame discovery
- Runtime-selectable data association
      - KD-tree Greedy association
      - Hungarian optimal assignment
- Association benchmarking framework
- Association runtime evaluation
- Constant-velocity Kalman filter state estimation
- Tuned Kalman filter parameters
- Standalone Kalman filter validation
- Configurable tracker parameters
- Prediction error evaluation
- Scenario-based benchmark generation
- Reproducible benchmark suite
- Trajectory export (CSV)
- Frame state export (CSV)
- Trajectory visualization
- Tracker debug visualization (CLI)

# ✅ Testing & Validation

The perception pipeline includes multiple layers of verification to ensure correctness and reproducibility.

Current validation includes:

- Hungarian assignment unit tests
- Regression tests derived from benchmark failures
- Synthetic benchmark scenarios
- Quantitative runtime benchmarking
- Prediction error evaluation
- Comparative evaluation between Greedy and Hungarian assignment

### Pipeline Architecture

```text
Scenario-based benchmark generation
          ↓
Frame Files
          ↓
Frame Loader
          ↓
Kalman State Estimation
          ↓
Data Association
   ├── Greedy (KD-tree)
   └── Hungarian
          ↓
Tracker Update
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

### Trajectory Visualization

![Trajectory Plot](perception_pipeline/output/trajectory_plot.png)

Shows the corrected Kalman filter state estimate for each tracked object across the entire sequence.

The visualization utility supports both trajectory plots and frame-level tracker debugging through a command-line interface, allowing different visualization modes without modifying the source code.

---

### Tracker Debug Visualization

![Tracker Debug View](perception_pipeline/output/tracker_frame_10.png)

Shows the internal state of the tracker for a single frame, including Kalman filter predictions, corrected state estimates, association errors, velocity estimates, and persistent track identities.

---

# 🛠️ How to Compile and Run

### Compile

```bash
clang++ -std=c++17 -Wall -Wextra Basics/calculator.cpp -o calculator
```

### Run

```bash
./calculator
```

---

# 🎯 Autonomous Systems Roadmap

## Phase 1 — C++ Fundamentals ✅

### Projects

- Hello Input
- Calculator
- Even/Odd
- Vector Average
- Max Number

### Skills

- Loops
- Conditionals
- Functions
- Vectors
- Input validation

---

## Phase 2 — Spatial Algorithms ✅

### Projects

- Distance Between Points
- Nearest Neighbor Search
- KD-Tree

### Skills

- Geometry
- Spatial search
- Recursion
- KD-tree construction
- Search optimization

---

## Phase 3 — Tracking Systems ✅

### Projects

- Multi-Frame Object Tracker

### Skills

- Persistent object identities
- Trajectory history
- KD-tree accelerated matching
- Lifecycle management
- State management over time

---

## Phase 4 — Perception Pipeline Foundations 🚧

### Completed

- File-based detection ingestion
- KD-tree accelerated data association
- Constant-velocity Kalman Filter state estimation
- Kalman parameter tuning
- Standalone Kalman filter validation
- Configurable tracker parameters
- Prediction error evaluation
- Scenario-based benchmark generation framework
- Benchmark suite documentation
- Aceelaeration benchmark
- Curved-motion benchmark
- Occlusion benchmark
- False-detection benchmark
- Cross-vehicle benchmark
- Trajectory visualization
- Tracker debug visualization
- Hungarian assignment
- Runtime-selectable association framework
- Association benchmarking
- Hungarian regression test suite
- Hungarian unit test suite

### Next Milestone

- Identity-switch evaluation
- Mahalanobis gating
- Adaptable association thresholds

---

# 🏁 Current Milestone

Completed

- ✅ KD-tree implementation
- ✅ Multi-frame tracking
- ✅ Constant-velocity prediction
- ✅ Benchmark generation
- ✅ Prediction error evaluation
- ✅ Trajectory visualization
- ✅ Tracker debug visualization
- ✅ Constant-velocity Kalman filter
- ✅ Visualization command-line interface
- ✅ Robust benchmark scenarios
- ✅ Hungarian assignment
- ✅ Runtime-selectable association framework
- ✅ Association benchmarking
- ✅ Hungarian regression test suite
- ✅ Hungarian unit test suite

Currently Working On

- 🚧 Identity-switch evaluation
- 🚧 Comparative association analysis

Future Goals

- ⬜ Adaptive Kalman filter tuning
- ⬜ Constant-acceleration motion model
- ⬜ Animated tracker visualization
- ⬜ OpenCV integration
- ⬜ Video-based perception
- ⬜ Real sensor integration

---

# 🚀 Current Focus

Building a perception pipeline foundation by separating:

```text
Scenario-based Benchmark Generator
          ↓
Frame Files
          ↓
Frame Loader
          ↓
Kalman State Estimation
          ↓
Data Association
          ↓
Tracking Update
          ↓
Evaluation
          ↓
Visualization
```

---

# 📌 Notes

- This repository focuses on incremental learning and systems development.
- Projects evolve from foundational C++ concepts into larger autonomy-oriented systems.
- All implementations are built from scratch to develop a deeper understanding of the underlying algorithms and architecture.
- The long-term goal is to build toward perception, tracking, and autonomous systems software.