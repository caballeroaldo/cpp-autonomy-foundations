from __future__ import annotations

import csv
from pathlib import Path
import sys
from typing import Dict, List, Tuple
import argparse

import matplotlib.pyplot as plt
from dataclasses import dataclass
from matplotlib.lines import Line2D

parser = argparse.ArgumentParser(description="Visualize tracking results.")

@dataclass
class FrameRecord:
    frame: int
    track_id: int

    pred_x: float
    pred_y: float

    x: float
    y: float

    vel_x: float
    vel_y:float

    prediction_error: float

def load_frame_data(csv_path: Path) -> List[FrameRecord]:
    records: List[FrameRecord] = []

    with csv_path.open("r", newline="") as f:
        reader = csv.DictReader(f)

        for row in reader:
            records.append(
                FrameRecord(
                    frame=int(row["frame"]),
                    track_id=int(row["track_id"]),

                    pred_x=float(row["pred_x"]),
                    pred_y=float(row["pred_y"]),

                    x=float(row["x"]),
                    y=float(row["y"]),

                    vel_x=float(row["vel_x"]),
                    vel_y=float(row["vel_y"]),

                    prediction_error=float(row["prediction_error"])
                )
            )
    
    return records

def list_available_frames(records: List[FrameRecord]) -> None:
    """Print the available frame numbers in frame_data.csv."""

    frame_counts = {}

    for record in records:
        frame_counts[record.frame] = (frame_counts.get(record.frame, 0) + 1)

    print("\nAvailable Frames")
    print("----------------")

    for frame in sorted(frame_counts):
        print(f"Frame {frame:>2} ({frame_counts[frame]} tracks)")

    print(f"\nTotal Frames: {len(frame_counts)}")

def read_track_csv(csv_path: Path) -> List[Tuple[int, float, float]]:
    points: List[Tuple[int, float, float]] = []

    with csv_path.open("r", newline="") as f:
        reader = csv.DictReader(f)
        required = {"frame", "x", "y"}

        if reader.fieldnames is None or not required.issubset(reader.fieldnames):
            raise ValueError(f"{csv_path.name} must contain columns: frame,x,y")

        for row in reader:
            frame = int(row["frame"])
            x = float(row["x"])
            y = float(row["y"])
            points.append((frame, x, y))

    points.sort(key=lambda t: t[0])
    return points


def load_tracks(output_dir: Path) -> Dict[str, List[Tuple[int, float, float]]]:
    tracks: Dict[str, List[Tuple[int, float, float]]] = {}

    csv_files = sorted(output_dir.glob("track_*.csv"))
    if not csv_files:
        raise FileNotFoundError(f"No track_*.csv files found in {output_dir}")

    for csv_file in csv_files:
        tracks[csv_file.stem] = read_track_csv(csv_file)

    return tracks

def friendly_track_name(track_stem: str, index: int) -> str:
    # Convert track_0 -> Vehicle A, track_1 -> Vehicle B, etc.
    return f"Vehicle {chr(ord('A') + index)}"

def plot_tracks(tracks: Dict[str, List[Tuple[int, float, float]]], save_path: Path) -> None:
    plt.figure(figsize=(12, 9))

    for idx, (track_name, points) in enumerate(tracks.items()):
        xs = [p[1] for p in points]
        ys = [p[2] for p in points]
        label = friendly_track_name(track_name, idx)

        plt.plot(xs, ys, marker="o", linewidth=2, label=label)

        # Label every point with its frame number
        for frame, x, y in points:
            plt.annotate(
                f"F{frame}",
                (x, y),
                textcoords="offset points",
                xytext=(4, 4),
                fontsize=6,
            )

    plt.title("Kalman Filter State Estimates\nKD-Tree Multi-Object Tracking")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    plt.gca().set_aspect("equal", adjustable="datalim")

    # Move legend outside the plot to keep it clean
    plt.legend(bbox_to_anchor=(1.05, 1), loc="upper left")
    plt.tight_layout()

    plt.savefig(save_path, dpi=200, bbox_inches="tight")
    plt.show()

def plot_tracker_frame( records: List[FrameRecord], frame_number: int , save_path: Path) -> None:
    """Visualize the tracker state for a single frame."""
    track_colors = [
        "tab:blue",
        "tab:orange",
        "tab:green",
        "tab:red",
        "tab:purple",
        "tab:brown",
        "tab:pink",
        "tab:gray",
    ]

    velocity_scale = 0.5
    
    legend_elements = [
    Line2D(
        [0], [0],
        marker="x",
        color="black",
        linestyle="None",
        label="Prediction",
        markersize=8,
    ),

    Line2D(
        [0], [0],
        marker="o",
        color="black",
        linestyle="None",
        label="Corrected Track",
        markersize=8,
    ),

    Line2D(
        [0], [0],
        color="black",
        linestyle="--",
        label="Association",
    )
]

    frame_records = [
        r for r in records
        if r.frame == frame_number
    ]

    if not frame_records:
        print(f"No data found for frame {frame_number}")
        return

    plt.figure(figsize=(10, 10))

    for record in frame_records:
        color = track_colors[record.track_id % len(track_colors)]

        plt.scatter(
            record.pred_x,
            record.pred_y,
            marker="x",
            s=120,
            linewidths=2,
            color=color,
            zorder=1
        )

        plt.scatter(
            record.x,
            record.y,
            marker="o",
            s=100,
            color=color,
            edgecolors="black",
            linewidths=0.8,
            zorder=3
        )

        plt.plot(
            [record.pred_x, record.x],
            [record.pred_y, record.y],
            color=color,
            linestyle="--",
            linewidth=1,
            alpha=0.35,
            zorder=0
        )

        plt.arrow(
            record.x,
            record.y,
            record.vel_x * velocity_scale,
            record.vel_y * velocity_scale,
            width=0.2,
            head_width=3,
            head_length=4,
            length_includes_head=True,
            color=color,
            zorder=2
        )

        plt.text(
            record.x + 2,
            record.y + 5,
            f"#{record.track_id}",
            fontsize=10,
            weight="bold",
            bbox=dict(
                facecolor="white",
                edgecolor="none",
                alpha=0.7,
                pad=1
            ),
            zorder=4
        )

    plt.title(f"KD-Tree Tracker Debug View\nFrame {frame_number}")
    plt.xlabel("X Position")
    plt.ylabel("Y Position")
    plt.legend(handles=legend_elements, bbox_to_anchor=(1.05,1), loc="upper left")
    plt.grid(True)
    plt.gca().set_aspect("equal", adjustable="datalim")
    plt.tight_layout()
    plt.savefig(save_path, dpi=200, bbox_inches="tight")
    plt.show()
    

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Visualize perception pipeline tracking results."
        )

    parser.add_argument(
        "--tracker",
        action="store_true",
        help="Generate the tracker debug visualization."
        )

    parser.add_argument(
        "--frame",
        type=int,
        default=10,
        help="Frame number for tracker visualization."
        )

    parser.add_argument(
        "--list-frames",
        action="store_true",
        help="List all avaiable frame numbers in frame_data.csv"
    )

    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="Directory containing exported CSV files."
    )

    args = parser.parse_args()
    script_dir = Path(__file__).resolve().parent
    if args.output is None:
        output_dir = script_dir / "output"
    else:
        output_dir = args.output.resolve()

    try:
        if args.list_frames:
            frame_records = load_frame_data( output_dir / "frame_data.csv" )
            list_available_frames(frame_records)
            return 0
        elif args.tracker:
            frame_records = load_frame_data(
                output_dir / "frame_data.csv"
            )
            tracker_save_path = (
                output_dir /
                f"tracker_frame_{args.frame}.png"
            )
            plot_tracker_frame(
                frame_records,
                args.frame,
                tracker_save_path,
            )
        else:
            trajectory_save_path = (
                output_dir /
                "trajectory_plot.png"
            )
            tracks = load_tracks(output_dir)
            plot_tracks(
                tracks,
                trajectory_save_path,
            )
            print(f"Saved plot to: {trajectory_save_path}")
        return 0

    except Exception as e:
        print(f"Error: {e}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())