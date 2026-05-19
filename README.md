# C++ Practice & Autonomous Systems Preparation

This repository contains my C++ practice programs and projects as I build toward developing systems for autonomous driving and robotics.

The goal of this repo is to strengthen:
- Core C++ programming skills
- Data structures and algorithms
- Systems thinking
- Foundations for perception and tracking pipelines

---

## 📁 Repository Structure
```
cpp-practice/
├── Basics/
├── DataStructures/
├── Tracking/
```

### Basics/
Introductory programs focused on core C++ concepts:
- Input/output (cin, cout)
- Control flow (if, switch, loops)
- Basic problem solving

Current programs:
- hello.cpp – user input and output
- calculator.cpp – basic arithmetic with error handling
- even-odd.cpp - determining if user input is an even or odd integer
- vector-average.cpp - finding average of an array of numbers using vectors

---

### DataStructures/
Programs focused on building fundamental data structures and algorithms.

Current programs:
- distance-two-points.cpp - outputs the distance between two (x,y) coordinates
- nearest-point.cpp - returns the nearest point in a collection of points to a queried point
- kdtree.cpp - Implements a 2D KD-Tree to optimize nearest neighbor search using recursive spatial parititioning and pruning

---

### Tracking/
Projects focused on object tracking and state management across frames.

Programs:
- Multi-Frame Object Tracker ⭐  
  A modular C++ tracking system that maintains object identities across sequential frames using KD-tree accelerated nearest-neighbor matching, trajectory history, stale-track deletion, and frame-aware observations.

Planned:
- Bounding box utilities (IoU)


---

## 🛠️ How to Compile and Run

Compile:
clang++ -std=c++17 -Wall -Wextra Basics/calculator.cpp -o calculator

Run:
./calculator

---

## 🎯 Learning Goals

This repository is part of a larger roadmap to prepare for:
- Autonomous driving software roles
- Perception and tracking systems
- High-performance and systems-level programming in C++

---

## 🚀 Next Steps

- Expand C++ fundamentals
- Implement KD-Tree for efficient spatial search
- Build a simple object tracking system
- Transition into full perception pipelines (C++ + Python)

---

## 📌 Notes

- This repo focuses on clarity and incremental learning
- Programs are intentionally simple and built from scratch
- Future projects will build on these foundations
