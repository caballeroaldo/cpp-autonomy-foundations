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

## Dataset: prediction_demo_false

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

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

## Dataset: prediction_demo_occlusion

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

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

## Dataset: prediction_demo_crossing

### Greedy

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

### Hungarian

| Metric | Value |
|---------|------:|
| Tracks Created | |
| Tracks Deleted | |
| Successful Associations | |
| Missed Associations | |
| Average Prediction Error | |
| Maximum Prediction Error | |
| Final Tracks | |
| Runtime (ms) | |

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

## Greedy

### Advantages

-

-

-

### Disadvantages

-

-

-

---

## Hungarian

### Advantages

-

-

-

### Disadvantages

-

-

-

---

# Final Assessment

Summary:

- Which algorithm performs better on clean datasets?
- Which algorithm performs better under ambiguity?
- Is the additional computational cost of Hungarian justified?
- Which algorithm would be recommended for future development?

---