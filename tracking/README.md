# Multi-Frame Object Tracker (C++)

This project implements a modular multi-frame object tracker in C++ that maintains object identities across sequential frames using KD-tree accelerated nearest-neighbor matching.

The tracker was developed as part of a larger roadmap toward autonomous systems and perception engineering, focusing on spatial data structures, state management, and real-time systems thinking.

---

# Project Goals

The goal of this project was to progressively build a simplified perception-style tracking pipeline from scratch using modern C++ concepts and spatial algorithms.

The tracker evolves through several stages:

1. Brute-force nearest-neighbor matching
2. KD-tree accelerated spatial search
3. Multi-frame tracking with persistent IDs
4. Track lifecycle management
5. Trajectory history and frame-aware observations

---

# Features

## Tracking Features

- Multi-frame object tracking
- Persistent track identities
- Frame-aware trajectory history
- New track creation for unseen objects
- Missed-frame handling
- Automatic stale-track deletion

---

## Spatial Search Features

- KD-tree accelerated nearest-neighbor matching
- Recursive spatial partitioning
- Axis-aligned pruning
- Approximate `O(log n)` average spatial queries

---

## Software Engineering Features

- Modular multi-file C++ architecture
- Shared data structures (`Point`, `Track`, `Observation`)
- Separation of KD-tree and tracking logic
- Helper-based output formatting
- Manual memory cleanup for KD-tree nodes

---

# Project Structure

```text
Tracking/
├── main.cpp
├── tracker.hpp
├── tracker.cpp
├── kdtree.hpp
├── kdtree.cpp
├── types.hpp
└── README.md

--- 

# File Responsibilities

## `types.hpp`

Defines shared data structures used across the tracker:

- `Point`
- `Observation`
- `Track`

---

## `kdtree.hpp / kdtree.cpp`

Contains:
- KD-tree construction
- Recursive nearest-neighbor search
- Spatial partitioning logic
- Tree cleanup and memory management

---

## `tracker.hpp / tracker.cpp`

Contains:
- Track printing helpers
- Matching utilities
- Trajectory formatting
- Output helpers

---

## `main.cpp`

Handles:
- Frame processing loop
- Detection input
- KD-tree integration
- Track lifecycle management

---

# KD-Tree Matching

The tracker originally used brute-force matching:

```text
for each detection:
    compare against every track
```

This approach scales poorly as the number of tracks increases.

To improve spatial query performance, the tracker was upgraded to use a KD-tree for nearest-neighbor search.

The KD-tree:
- recursively partitions space
- alternates between x/y splitting axes
- prunes regions that cannot contain closer points

This reduces average nearest-neighbor query complexity from:

```text
O(n)
```

to approximately:

```text
O(log n)
```

for balanced spatial distributions.

---

# Example Demo Run

## Input

```text
Frame 1:
(100,200)
(300,400)

Frame 2:
(105,205)
(298,398)
(700,700)

Frame 3:
(110,210)
(705,705)
```

---

## Output

```text
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

# Tested Scenarios

The tracker was tested against several tracking scenarios, including:

- Basic one-to-one matching
- New object creation
- Temporary object disappearance
- Stale-track deletion
- Dense point clusters
- Crossing-object scenarios
- Far-distance threshold rejection

---

# Current Limitations

The current implementation intentionally keeps the matching strategy simple.

Known limitations:

- Greedy matching is not globally optimal
- Terminal-based detections only
- No motion prediction (Kalman filtering)
- No OpenCV or graphical visualization yet
- No Hungarian assignment optimization

---

# Future Improvements

Planned upgrades include:

- Hungarian algorithm assignment
- Kalman filter trajectory prediction
- OpenCV trajectory visualization
- Video/simulation-based detections
- Real-time perception pipeline integration

---

# Learning Outcomes

This project strengthened understanding of:

- KD-tree spatial data structures
- Recursive nearest-neighbor search
- Multi-frame object tracking
- State management over time
- Spatial query optimization
- Real-time systems thinking
- Modular C++ software architecture

The tracker serves as a foundational step toward larger perception and autonomous systems projects.