# Benchmark Results

# Experimental Conditions

Unless otherwise noted, all benchmarks were evaluated using:

- Tuned constant-velocity Kalman filter
- KD-tree nearest-neighbor data association
- Identical tracker configuration
- Synthetic frame sequences generated with deterministic random seeds

## Current Results

| Dataset | Primary System Tested | Avg Prediction Error | Max Error | Successful Associations | Missed Associations |
|----------|--------------------:|--------------------:|----------:|------------------------:|--------------------:|
| prediction_demo_clean | Baseline | 4.52 | 21.26 | 47 | 2 |
| acceleration_demo | Motion Model (Speed) | 7.99 | 25.18 | 47 | 2 |
| curved_demo | Motion Model (Heading) | 8.16 | 25.18 | 47 | 2 |
| occlusion_demo | Measurement Robustness | 8.27 | 25.18 | 45 | 2 | 
| false_detection_demo | Track Lifecycle | 7.93 | 25.18 | 47 | 5 |
| cross_demo | Data Association | 6.56 | 25.18 | 36 | 0 |

## Observations

### prediction_demo_clean

Provides the baseline performance for all subsequent benchmark comparisons.

---

### acceleration_demo

The tuned constant-velocity Kalman filter remained robust under moderate acceleration. Prediction error increased while successful associations remained unchanged.

---

### curved_demo

Gradual changes in vehicle heading produced higher prediction error than the baseline while preserving stable data association and object identities.

---

### occlusion_demo

The tracker maintained the existing track through a two-frame occlusion and correctly reassociated the returning detection with its original track. No identity switches occurred.

---

### false_detection_demo

Temporary tracks were created for isolated false detections and automatically removed after exceeding the missed-frame threshold. Legitimate tracks were unaffected.

---

### crossing_demo

Two vehicles crossed near the center of the scene without producing identity switches or association failures. Under this level of trajectory separation, greedy nearest-neighbor association remained sufficient.

## Planned Benchmarks

- Closely spaced vehicle interactions
- Stop-and-go traffic
- Dense multi-object intersections
- Multi-lane overtaking scenarios