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
├── data-structures/
├── tracking/
├── perception-pipeline/
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

### Planned Work

- File-based frame ingestion
- Detection pipeline architecture
- Frame loaders and parsers
- Trajectory export
- Visualization tooling
- Simulation integration

### Goal

```text
Detection Source
       ↓
Frame Loader
       ↓
KD-Tree Matching
       ↓
Tracker
       ↓
Trajectory Output
```

This phase serves as the bridge between algorithm development and full perception systems.

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

### Current Focus

- File-based detection ingestion
- Pipeline architecture
- Detection-to-tracking integration
- Trajectory export
- Visualization preparation

### Future

- Simulation inputs
- OpenCV integration
- Real-time perception systems

---

# 🚀 Current Focus

Building a perception pipeline foundation by separating:

```text
Detection Source
       ↓
Tracking System
```

and moving toward:

```text
Detection Source
       ↓
Perception Pipeline
       ↓
Tracking System
       ↓
Trajectory Analysis
```

---

# 📌 Notes

- This repository focuses on incremental learning and systems development.
- Projects evolve from foundational C++ concepts into larger autonomy-oriented systems.
- All implementations are built from scratch to develop a deeper understanding of the underlying algorithms and architecture.
- The long-term goal is to build toward perception, tracking, and autonomous systems software.