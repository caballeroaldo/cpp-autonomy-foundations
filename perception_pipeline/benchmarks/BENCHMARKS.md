# Perception Pipeline Benchmark Suite

This document describes the synthetic benchmark datasets used to evaluate the multi-object tracking pipeline.

Each benchmark is designed to isolate a specific challenge while keeping the remaining scene characteristics unchanged. This allows quantitative comparisons between tracking algorithms and motion models.

| Dataset | Purpose | Status |
|----------|---------|--------|
| prediction_demo_clean | Baseline constant-velocity motion without measurement noise | ✅ |
| prediction_demo_noisy | Baseline with Gaussian measurement noise | ✅ |
| acceleration_demo | Tests robustness to increasing vehicle speed | ✅ |
| curved_demo | Tests robustness to gradual heading changes | ✅ |
| occlusion_demo | Temporary loss of measurements | ✅ |
| crossing_demo | Vehicles crossing paths | ✅ |
| false_detection_demo | Random false measurements | ✅ |

### prediction_demo_clean

Purpose

Provide a baseline dataset with constant-velocity motion and noise-free detections.

Expected Behavior

- Lowest prediction error
- Stable associations
- Smooth trajectories

Primary Evaluation Metrics

- Average prediction error
- Maximum prediction error
- Successful associations

### prediction_demo_noisy

Purpose

Evaluate robustness to Gaussian measurement noise while maintaining constant-velocity motion.

Expected Behavior

- Increased prediction error
- Stable tracking
- Minor trajectory jitter

### acceleration_demo

Purpose

Challenge the constant-velocity assumption by introducing gradual acceleration for one vehicle.

Modified Object

- Eastbound vehicle

Motion

x = x₀ + vt + ½at²

Expected Behavior

- Increased prediction error
- Stable associations
- No identity switches

### curved_demo

Purpose

Challenge the constant-velocity assumption by gradually changing the vehicle heading.

Modified Object

- Eastbound vehicle

Expected Behavior

- Increased prediction error
- Smooth trajectory curvature
- Stable track identities

## occlusion_demo

Purpose

Evaluate the ability of the tracker to maintain object identities during temporary measurement loss.

Challenge Introduced

- Two-frame detection dropout
- Constant-velocity motion
- Gaussian measurement noise

Expected Behavior

- Kalman filter predicts through missing measurements
- Track identity is preserved
- Vehicle is correctly reassociated when detections return

## false_detection_demo

Purpose

Evaluate the tracker when isolated false positive detections are introduced.

Challenge Introduced

- Single-frame false detections
- Constant-velocity motion
- Gaussian measurement noise

Expected Behavior

- Temporary tracks are created
- False tracks are automatically deleted after exceeding the missed-frame threshold
- Legitimate track identities remain unaffected

## crossing_demo

Purpose

Evaluate the tracker when two vehicles pass through the same region of space simultaneously.

Challenge Introduced

- Closely spaced detections
- Potential association ambiguity
- Constant-velocity motion
- Gaussian measurement noise

Expected Behavior

- Vehicle identities remain consistent
- No unnecessary track creation
- Stable data association through the intersection