# C++ Practice & Autonomous Systems Preparation

This repository contains my C++ projects and systems-focused engineering work as I build toward developing software for autonomous driving, perception, robotics, and real-time systems.

The goal of this repository is to strengthen:

- Core C++ programming skills
- Data structures and algorithms
- Systems thinking
- Spatial reasoning and tracking
- Foundations for perception pipelines

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

- KD-tree accelerated nearest-neighbor matching
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

### Current Features

- File-based frame ingestion
- Automatic frame discovery
- Detection-to-tracker integration
- Synthetic traffic generation
- Trajectory export (CSV)
- Python trajectory visualization

### Pipeline Architecture

```text
Synthetic Traffic Generator
          ↓
Frame Files
          ↓
Frame Loader
          ↓
KD-Tree Matching
          ↓
Tracker
          ↓
Trajectory Export
          ↓
Trajectory Visualization
```

### Example Visualization

Synthetic traffic tracking scenario generated using the perception pipeline
![Synthetic Traffic Tracking Demo](perception_pipeline/output/trajectory_plot.png)

Each colored trajectory represents a tracked object. Frame labels indicate temporal progression and demonstrate persistent object identity across sequential observations.

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
- Automatic frame discovery
- Detection-to-tracking integration
- Trajectory export (CSV)
- Synthetic traffic generation
- Trajectory visualization

### Next Steps

- More realistic traffic simulation
- Motion prediction
- OpenCV integration
- Video-based detections

---

# 🏁 Current Milestone

Completed:

- ✅ KD-Tree implementation
- ✅ Multi-frame object tracking
- ✅ KD-tree accelerated matching
- ✅ Modular tracker architecture
- ✅ Perception pipeline foundations
- ✅ Synthetic traffic generation
- ✅ Trajectory export (CSV)
- ✅ Trajectory visualization

Currently Working On:

- 🚧 More realistic traffic simulation
- 🚧 Motion prediction concepts
- 🚧 Perception system expansion

Future Goals:

- ⬜ Kalman filtering
- ⬜ OpenCV integration
- ⬜ Video-based detections
- ⬜ Full perception pipeline

---

# 🚀 Current Focus

Building a perception pipeline foundation by separating:

```text
Synthetic Traffic Generator
          ↓
Frame Files
          ↓
Frame Loader
          ↓
Tracking System
          ↓
Trajectory Export
          ↓
Visualization
```

---

# 📌 Notes

- This repository focuses on incremental learning and systems development.
- Projects evolve from foundational C++ concepts into larger autonomy-oriented systems.
- All implementations are built from scratch to develop a deeper understanding of the underlying algorithms and architecture.
- The long-term goal is to build toward perception, tracking, and autonomous systems software.