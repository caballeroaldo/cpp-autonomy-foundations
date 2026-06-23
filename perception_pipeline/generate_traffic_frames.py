#!/usr/bin/env python3
from __future__ import annotations

import argparse
import random
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, List, Tuple


@dataclass(frozen=True)
class Trajectory:
    name: str
    start_frame: int
    end_frame: int
    x_fn: Callable[[int], float]
    y_fn: Callable[[int], float]


def jitter(name: str, frame: int, amplitude: int = 1) -> Tuple[int, int]:
    rng = random.Random(f"{name}:{frame}")
    return rng.randint(-amplitude, amplitude), rng.randint(-amplitude, amplitude)


# A more intersection-like synthetic traffic scene
TRAJECTORIES: List[Trajectory] = [
    # Eastbound car
    Trajectory(
        name="eastbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 40 + 18 * t,
        y_fn=lambda t: 150,
    ),
    # Northbound car
    Trajectory(
        name="northbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 205,
        y_fn=lambda t: 40 + 16 * t,
    ),
    # Westbound car
    Trajectory(
        name="westbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 450 - 20 * t,
        y_fn=lambda t: 190 - 4 * t,
    ),
    # Southbound car
    Trajectory(
        name="southbound",
        start_frame=1,
        end_frame=10,
        x_fn=lambda t: 275,
        y_fn=lambda t: 340 - 18 * t,
    ),
    # Turning vehicle
    Trajectory(
        name="turning_vehicle",
        start_frame=3,
        end_frame=10,
        x_fn=lambda t: 120 + 14 * t,
        y_fn=lambda t: 330 - 15 * t - 1.5 * (t * t),
    ),
    # Late merge / late entrant
    Trajectory(
        name="late_merge",
        start_frame=6,
        end_frame=10,
        x_fn=lambda t: 70 + 15 * t,
        y_fn=lambda t: 255 - 10 * t,
    ),
]


def generate_points_for_frame(frame: int) -> List[Tuple[int, int]]:
    points: List[Tuple[int, int]] = []

    for traj in TRAJECTORIES:
        if traj.start_frame <= frame <= traj.end_frame:
            local_t = frame - traj.start_frame
            x = traj.x_fn(local_t)
            y = traj.y_fn(local_t)
            jx, jy = jitter(traj.name, frame, amplitude=1)
            points.append((int(round(x + jx)), int(round(y + jy))))

    # Stable output order
    points.sort(key=lambda p: (p[0], p[1]))
    return points


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate synthetic traffic frame files.")
    parser.add_argument(
        "--output",
        type=str,
        default="frames/intersection_demo",
        help="Output directory for frame_XX.txt files",
    )
    parser.add_argument(
        "--frames",
        type=int,
        default=10,
        help="Number of frames to generate",
    )
    parser.add_argument(
        "--clear",
        action="store_true",
        help="Clear existing frame_*.txt files in the output directory first",
    )

    args = parser.parse_args()

    output_dir = Path(args.output)
    output_dir.mkdir(parents=True, exist_ok=True)

    if args.clear:
        for old_file in output_dir.glob("frame_*.txt"):
            old_file.unlink()

    for frame in range(1, args.frames + 1):
        points = generate_points_for_frame(frame)
        out_path = output_dir / f"frame_{frame:02d}.txt"

        with out_path.open("w", encoding="utf-8") as f:
            for x, y in points:
                f.write(f"{x} {y}\n")

        print(f"Wrote {out_path} ({len(points)} detections)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())