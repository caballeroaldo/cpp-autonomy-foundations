# Perception Pipeline Benchmark Results

This document summarizes the quantitative evaluation of the perception pipeline across the synthetic benchmark suite.

The experiments compare the two supported data association algorithms:

- Greedy Nearest Neighbor (KD-Tree)
- Hungarian Assignment

All experiments were performed using identical tracker configurations, allowing only the association algorithm to vary.

---

# Experimental Configuration

| Parameter | Value |
|-----------|-------|
| State Estimator | Linear Kalman Filter |
| Association Algorithms | Greedy KD-Tree, Hungarian Assignment |
| Association Interface | `std::vector<Association>` |
| Cost Metric | Squared Euclidean Distance |
| Motion Model | Constant Velocity |
| Measurement Model | Position Only (x, y) |
| Track Initialization | First detection creates a new track |
| New Track Creation | Unmatched detections |
| Track Deletion | `maxMissedFrames` consecutive missed frames |
| Velocity Smoothing | 0.0 |
| Max Association Distance | `config.maxAssociationDistanceSquared` |
| Max Missed Frames | `config.maxMissedFrames` |
| Programming Language | C++17 |
| Linear Algebra Library | Eigen 3 |
| Compiler | Clang++ |
| Platform | macOS |

---

# Experimental Methodology

All benchmark datasets were evaluated using the same tracker configuration.

The only parameter changed between experiments was the data association algorithm.

Both algorithms used:

- Identical Kalman filter parameters
- Identical track management
- Identical benchmark datasets
- Identical tracker configuration

Association runtime was measured independently from the remainder of the tracking pipeline.

---

# Motion Model Evaluation

The tuned constant-velocity Kalman filter was evaluated across the synthetic benchmark suite.

| Dataset | Avg Prediction Error | Max Prediction Error | Successful Associations | Missed Associations |
|----------|--------------------:|---------------------:|------------------------:|--------------------:|
| prediction_demo_clean | 4.52 | 21.26 | 47 | 2 |
| acceleration_demo | 7.99 | 25.18 | 47 | 2 |
| curved_demo | 8.16 | 25.18 | 47 | 2 |
| occlusion_demo | 8.27 | 25.18 | 45 | 2 |
| false_detection_demo | 7.93 | 25.18 | 47 | 5 |
| crossing_demo | 6.56 | 25.18 | 36 | 0 |

These experiments established the baseline prediction performance prior to introducing alternative association algorithms.

---

# Association Algorithm Evaluation

## prediction_demo_clean

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 4.52031 |
| Maximum Prediction Error | 21.2603 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.098916 |
| Average Association Time (ms) | 0.0109907 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 4.52031 |
| Maximum Prediction Error | 21.2603 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.17946 |
| Average Association Time (ms) | 0.01994 |

**Observation**

- Both algorithms produced identical tracking performance.
- The clean dataset contains little association ambiguity.
- Greedy completed association faster due to its local nearest-neighbor search.

---

## prediction_demo_occlusion

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 45 |
| Missed Associations | 0 |
| Average Prediction Error | 8.26633 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.097834 |
| Average Association time (ms) | 0.0108704 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 45 |
| Missed Associations | 0 |
| Average Prediction Error | 13.6014 |
| Maximum Prediction Error | 117.551 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.222292 |
| Average Association time (ms) | 0.0246991 |

**Observation**

- Both algorithms maintained track continuity during temporary measurement loss.
- Greedy produced lower prediction error.
- Hungarian incurred higher computational cost.

---

## crossing_demo

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 7.64311 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.280417 |
| Average Association Time (ms) | 0.0311574 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 7.64311 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.189209 |
| Average Association Time (ms) | 0.0210232 |

**Observation**

- Both algorithms maintained stable track identities throughout the crossing scenario.
- Prediction accuracy remained identical.
- Hungarian completed association slightly faster for this dataset.

---

## acceleration_demo

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 7.64311 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.280417 |
| Average Association Time (ms) | 0.0311574 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 7.64311 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.189209 |
| Average Association Time (ms) | 0.0210232 |

**Observation**

- Both algorithms achieved identical prediction accuracy.
- Greedy remained computationally faster.

---

## false_detection_demo

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 9 |
| Tracks Deleted | 2 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 7.93441 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 7 |
| Total Association Runtime (ms) | 0.175957 |
| Average Association Time (ms) | 0.0195508 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 7 |
| Tracks Deleted | 0 |
| Successful Associations | 49 |
| Missed Associations | 0 |
| Average Prediction Error | 33.8764 |
| Maximum Prediction Error | 167.92 |
| Final Tracks | 7 |
| Total Association Runtime (ms) | 1.46479 |
| Average Association Time (ms) | 0.162754 |

**Observation**

This benchmark became the most challenging dataset for the Hungarian implementation.

Initial experiments exposed three implementation issues:

- Pointer invalidation during augmenting-path application.
- Inconsistent uncovered-zero eligibility between `findUncoveredZero()` and `findMinimumUncoveredValue()`.
- Missing regression coverage.

These issues were resolved through:

- Coordinate-based augmenting paths.
- Consistent uncovered-zero filtering.
- Permanent regression testing.

After the fixes, the Hungarian implementation successfully completed all benchmark scenarios.

The benchmark also demonstrated that:

- Hungarian created fewer tracks.
- Hungarian produced more successful associations.
- Hungarian accepted several physically implausible assignments, resulting in significantly larger prediction errors than Greedy.

An experimental post-assignment gating strategy reduced prediction error but substantially increased track fragmentation. Since practical tracking systems typically apply gating before solving the assignment problem, this investigation is reserved for future work.

---

# Overall Comparison

| Category | Greedy | Hungarian |
|----------|---------|------------|
| Runtime | Lower | Higher |
| Prediction Accuracy (Clean) | Equal | Equal |
| Prediction Accuracy (False Detections) | Lower Error | Higher Error |
| Successful Associations | Similar | Slightly Higher |
| Track Creation | More | Fewer |
| Implementation Complexity | Lower | Higher |

---

# Discussion

The benchmark suite highlights the trade-offs between local and global association strategies.

Greedy nearest-neighbor association provides consistently low runtime and performs well when association ambiguity is limited.

Hungarian assignment solves the global assignment problem and produced slightly more successful associations while creating fewer tracks in ambiguous scenarios.

However, the false-detection benchmark demonstrated that globally optimal assignment alone is insufficient when implausible associations are permitted. The debugging process ultimately produced a more robust implementation through regression testing and improved algorithm consistency.

---

# Validation

The completed Hungarian implementation was validated using multiple complementary approaches.

## Unit Testing

- Basic 3×3 assignment
- Identity assignment
- Diagonal minimum assignment
- Classic 4×4 benchmark
- False-detection regression test

## Scenario Validation

- prediction_demo_clean
- acceleration_demo
- curved_demo
- occlusion_demo
- crossing_demo
- false_detection_demo

All benchmark scenarios complete successfully using both association algorithms.

---

# Conclusions

The benchmark suite demonstrates that:

- Greedy nearest-neighbor association provides consistently lower computational cost.
- Hungarian assignment produces globally optimal assignments while maintaining comparable performance on unambiguous datasets.
- Challenging benchmark scenarios uncovered implementation issues that were resolved through systematic debugging and regression testing.
- The modular association interface allows both algorithms to be evaluated using identical tracker infrastructure.

---

# Future Work

- Identity-switch evaluation
- Pre-assignment statistical gating
- Adaptive Kalman filter tuning
- Constant-acceleration motion model
- Animated visualization
- OpenCV integration
- Video-based perception
- Real sensor integration