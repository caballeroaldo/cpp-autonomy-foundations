# Multi-Object Tracking Association Comparison

## Project

Comparison of two data association algorithms integrated into the perception pipeline:

- **Greedy Nearest Neighbor (KD-Tree)**
- **Hungarian Assignment**

---

# System Configuration

| Parameter | Value |
|-----------|-------|
| State Estimator | Linear Kalman Filter |
| Association Algorithms | Greedy Nearest Neighbor (KD-Tree), Hungarian Assignment |
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
| Build Compiler | Clang++ |
| Platform | macOS |
| Date | 08-18-2026 |

## Experimental Methodology

- The same Kalman Filter configuration was used for all experiments.
- The same tracker parameters were used for both association algorithms.
- Only the association method (Greedy or Hungarian) was changed between runs.
- All datasets were processed using identical tracker settings.
- Results were collected using the runtime command-line selection:
  - `./perception_pipeline <dataset> greedy`
  - `./perception_pipeline <dataset> hungarian`

---

# Dataset Results

---

## Dataset: prediction_demo_clean

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
| Total Runtime (ms) | 0.17946 |
| Average Frame Time (ms) | 0.01994 |

### Observations

**Greedy**

-

-

-

**Hungarian**

-

-

-

---

## Dataset: false_detection_demo

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

### Observations

**Greedy**

**Hungarian**

- Due to the false detections, the logic is not able to complete assignments, leading to the program to abort before finishing. This suggests a different approach in the Hungarian assignment is necessary to remedy edge cases like false detections

**Outcome**

- During evaluation on the false-detection dataset, the Hungarian implementation exposed an infinite loop. Investigation revealed three independent issues:
pointer invalidation during augmenting path application,
inconsistent eligibility criteria between findUncoveredZero() and findMinimumUncoveredValue(),
regression cases not covered by the original unit tests.
These were resolved through coordinate-based augmenting paths and consistent uncovered-zero filtering, after which all datasets completed successfully.

---

## Dataset: prediction_demo_occlusion

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

### Observations

**Greedy**

- Greedy performed better than Hungarian in the prediction errors (Avg and Max) in addtion to the Association runtime (Total and Average)

---

## Dataset: cross_demo

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

### Observations

**Greedy**

-

-

-

**Hungarian**

-

-

-

---

## Dataset: acceleration_demo

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 8.38343 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.134168 |
| Average Association Time (ms) | 0.0149076 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 8.38343 |
| Maximum Prediction Error | 25.1794 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.179332 |
| Average Association Time (ms) | 0.0199258 |

### Observations

**Greedy**

- Greedy performed better than Hungarian in the runtime (total and average) while staying the same in the prediction error

---

## Dataset: intersection_demo

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 5.62595 |
| Maximum Prediction Error | 21.4009 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.135915|
| Average Association Time (ms) | 0.0151017 |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | 6 |
| Tracks Deleted | 0 |
| Successful Associations | 47 |
| Missed Associations | 0 |
| Average Prediction Error | 5.62595 |
| Maximum Prediction Error | 21.4009 |
| Final Tracks | 6 |
| Total Association Runtime (ms) | 0.168126 |
| Average Association Time (ms) | 0.0186807 |

### Observations

**Greedy**

- Similar to the dataset acceleration_demo, the runtime for Greedy performs better than Hungarian, while staying the same in the other stats.


---

## Dataset: prediction_demo_noisy2

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
| Total Association Runtime (ms) | 0.111624 |
| Average Association Time (ms) | 0.0124027 |

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
| Total Association Runtime (ms) | 0.236669 |
| Average Association Time (ms) | 0.0262966 |

### Observations

**Greedy**

- Greedy finishes faster than Hungarian

---

# Overall Comparison

| Metric | Greedy | Hungarian | Better |
|---------|--------:|----------:|:------|
| Tracks Created | | | |
| Tracks Deleted | | | |
| Successful Associations | | | |
| Missed Associations | | | |
| Average Prediction Error | | | |
| Maximum Prediction Error | | | |
| Runtime | | | |

---

# Qualitative Comparison

| Category | Greedy | Hungarian |
|----------|---------|------------|
| Ease of Implementation | | |
| Runtime Complexity | | |
| Handles Ambiguous Matches | | |
| Handles Crossing Targets | | |
| Handles Occlusions | | |
| Handles False Detections | | |
| Code Complexity | | |
| Overall Robustness | | |

---

# Conclusions

- Upon further investigation, despite Hungarian assignment did not need to delete tracks, whereas Greedy did, Greedy continued to have a lower prediction error in comparison to Hungarian. Greedy in fact had a gate threshold that limited the track assignment based on that squared distance whereas that gate was not present in the Hungarian implementation. Next steps would be to compare the Hungarian implementation without and with the same gate threshold to ensure that was causing larger prediction errors while simultaneously having better assignment to Greedy.
- However, experiments with a post-assignment gate reduced prediction error but significantly increased track fragmentation. In practical multi-object tracking systems, gating is typically incorporated before the Hungarian assignment by invalidating infeasible associations in the cost matrix rather than rejecting assignments afterward.

---

# Final Assessment

Summary:

- Which algorithm performs better on clean datasets?
- Which algorithm performs better under ambiguity?
- Is the additional computational cost of Hungarian justified?
- Which algorithm would be recommended for future development?

---