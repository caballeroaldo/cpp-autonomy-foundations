#!/usr/bin/env python3
from __future__ import annotations

import argparse
import random
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, List, Tuple
from copy import deepcopy


@dataclass(frozen=True)
class Trajectory:
    # Persistent identity used only for evaluation
    # This identifier is never written to the tracker input.
    id: int
    name: str
    start_frame: int
    end_frame: int
    x_fn: Callable[[int], float]
    y_fn: Callable[[int], float]

@dataclass
class Scenario:
    name: str
    trajectories: List[Trajectory]
    occlusions: dict[str, set[int]]
    false_detections: dict[int, List[Tuple[int,int]]]

def add_measurement_noise(
    name: str,
    frame: int,
    noise_std: float = 0.0
) -> Tuple[float, float]:
    '''
    Adds deterministic Gaussian measurement noise.

    The random generator is seeded using the trajectory name and frame number so that 
    every experiment is perfectly repeatable
    '''
    rng = random.Random(f"{name}:{frame}")

    noise_x = rng.gauss(0.0, noise_std)
    noise_y = rng.gauss(0.0, noise_std)
    return noise_x, noise_y

# A more intersection-like synthetic traffic scene
BASE_TRAJECTORIES: List[Trajectory] = [
    # Eastbound car
    Trajectory(
        id=0,
        name="eastbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 40 + 18 * t,
        y_fn=lambda t: 150,
    ),

    # Northbound car
    Trajectory(
        id=1,
        name="northbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 205,
        y_fn=lambda t: 40 + 16 * t,
    ),

    # Westbound car
    Trajectory(
        id=2, 
        name="westbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 450 - 20 * t,
        y_fn=lambda t: 190 - 4 * t,
    ),

    # Southbound car
    Trajectory(
        id=3,
        name="southbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 275,
        y_fn=lambda t: 340 - 18 * t,
    ),

    # Turning vehicle
    Trajectory(
        id=4,
        name="turning_vehicle",
        start_frame=3,
        end_frame=10,
        x_fn=lambda t: 120 + 14 * t,
        y_fn=lambda t: 330 - 15 * t - 1.5 * (t * t),
    ),

    # Late merge / late entrant
    Trajectory(
        id=5,
        name="late_merge",
        start_frame=6,
        end_frame=10,
        x_fn=lambda t: 70 + 15 * t,
        y_fn=lambda t: 255 - 10 * t,
    ),
]

def prediction_scenario() -> Scenario:
    return Scenario(
        name="prediction",
        trajectories=deepcopy(BASE_TRAJECTORIES),
        occlusions={},
        false_detections={}
    )


def acceleration_scenario() -> Scenario:
    trajectories = deepcopy(BASE_TRAJECTORIES)

    trajectories[0] = Trajectory(
        id=0,
        name="eastbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 40 + 12 * t + 0.8 * (t * t),
        y_fn=lambda t: 150,
    )

    return Scenario(
        name="acceleration",
        trajectories=trajectories,
        occlusions={},
        false_detections={}
    )


def curved_scenario() -> Scenario:
    trajectories = deepcopy(BASE_TRAJECTORIES)

    trajectories[0] = Trajectory(
        id=0,
        name="eastbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 40 + 20 * t,
        y_fn=lambda t: 150 + 0.6 * (t * t),
    )

    return Scenario(
        name="curved",
        trajectories=trajectories,
        occlusions={},
        false_detections={}
    )


def occlusion_scenario() -> Scenario:
    return Scenario(
        name="occlusion",
        trajectories=deepcopy(BASE_TRAJECTORIES),
        occlusions={
            "eastbound": {5, 6}
        },
        false_detections={}
    )


def false_detection_scenario() -> Scenario:
    return Scenario(
        name="false_detection",
        trajectories=deepcopy(BASE_TRAJECTORIES),
        occlusions={},
        false_detections={
            3: [(430, 60)],
            6: [(80, 40)],
            8: [(350, 310)],
        }
    )


def crossing_scenario() -> Scenario:
    trajectories = deepcopy(BASE_TRAJECTORIES)

    trajectories[0] = Trajectory(
        id=0,
        name="eastbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 60 + 18 * t,
        y_fn=lambda t: 150,
    )

    trajectories[1] = Trajectory(
        id=1,
        name="northbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 205,
        y_fn=lambda t: 30 + 16 * t,
    )

    # Remove turning vehicle and late merge
    trajectories = [
        traj
        for traj in trajectories
        if traj.name not in {
            "turning_vehicle",
            "late_merge"
        }
    ]

    return Scenario(
        name="crossing",
        trajectories=trajectories,
        occlusions={},
        false_detections={}
    )

SCENARIOS = {
    "prediction": prediction_scenario,
    "acceleration": acceleration_scenario,
    "curved": curved_scenario,
    "occlusion": occlusion_scenario,
    "false_detection": false_detection_scenario,
    "crossing": crossing_scenario
}

def generate_points_for_frame(frame: int, scenario: Scenario, noise_std: float) -> List[Tuple[int, int]]:
    points: List[Tuple[int, int]] = []

    for traj in scenario.trajectories:
        if traj.start_frame <= frame <= traj.end_frame:
            if frame in scenario.occlusions.get(traj.name,set()):
                continue
            local_t = frame - traj.start_frame
            x = traj.x_fn(local_t)
            y = traj.y_fn(local_t)
            jx, jy = add_measurement_noise(traj.name,frame,noise_std)
            points.append((int(round(x + jx)), int(round(y + jy))))

    for detection in scenario.false_detections.get(frame, []):
        points.append(detection)

    # Stable output order
    points.sort(key=lambda p: (p[0], p[1]))
    return points


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate synthetic traffic frame files.")
    parser.add_argument(
        "--output",
        type=str,
        required=True,
        help="Output directory for frame_XX.txt files",
    )
    parser.add_argument(
        "--frames",
        type=int,
        default=10,
        help="Number of frames to generate",
    )
    parser.add_argument(
        "--noise",
        type=float,
        default=0.0,
        help="Standard deviation of Gaussian measurement noise (pixels)"
    )
    parser.add_argument(
        "--clear",
        action="store_true",
        help="Clear existing frame_*.txt files in the output directory first",
    )
    parser.add_argument(
        "--scenario",
        type=str,
        default="prediction",
        choices=SCENARIOS.keys(),
        help="Benchmark scenario to generate"
    )

    args = parser.parse_args()
    scenario = SCENARIOS[args.scenario]()

    output_dir = Path(args.output)
    output_dir.mkdir(parents=True, exist_ok=True)

    if args.clear:
        for old_file in output_dir.glob("frame_*.txt"):
            old_file.unlink()

    for frame in range(1, args.frames + 1):
        points = generate_points_for_frame(frame,scenario,args.noise)
        out_path = output_dir / f"frame_{frame:02d}.txt"

        with out_path.open("w", encoding="utf-8") as f:
            for x, y in points:
                f.write(f"{x} {y}\n")

        print(f"Wrote {out_path} ({len(points)} detections)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())