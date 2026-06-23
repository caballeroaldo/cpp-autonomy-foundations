from __future__ import annotations

import csv
from pathlib import Path
import sys
from typing import Dict, List, Tuple

import matplotlib.pyplot as plt


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

    plt.title("KD-Tree Accelerated Multi-Object Tracking")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    plt.gca().set_aspect("equal", adjustable="datalim")

    # Move legend outside the plot to keep it clean
    plt.legend(bbox_to_anchor=(1.05, 1), loc="upper left")
    plt.tight_layout()

    plt.savefig(save_path, dpi=200, bbox_inches="tight")
    plt.show()


def main() -> int:
    script_dir = Path(__file__).resolve().parent
    output_dir = script_dir / "output"
    save_path = output_dir / "trajectory_plot.png"

    if len(sys.argv) > 1:
        output_dir = Path(sys.argv[1]).resolve()
        save_path = output_dir / "trajectory_plot.png"

    try:
        tracks = load_tracks(output_dir)
        plot_tracks(tracks, save_path)
        print(f"Saved plot to: {save_path}")
        return 0
    except Exception as e:
        print(f"Error: {e}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())