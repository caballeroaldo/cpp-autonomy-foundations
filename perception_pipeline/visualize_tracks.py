from __future__ import annotations

import csv
from pathlib import Path
import sys
from typing import Dict, List, Tuple

import matplotlib.pyplot as plt

def read_track_csv(csv_path: Path) -> List[Tuple[int, float, float]]:
    '''
    Read a single track CSV in the format:
        frame,x,y
        1,100,200
        2,105,205
    '''
    points: List[Tuple[int, float, float]] = []

    with csv_path.open("r", newline="") as f:
        reader = csv.DictReader(f)
        required = {"frame", "x", "y"}

        if reader.fieldnames is None or not required.issubset(reader.fieldnames):
            raise ValueError(
                f"{csv_path.name} must contain columns: frame,x,y"
            )

        for row in reader:
            frame = int(row["frame"])
            x = float(row["x"])
            y = float(row["y"])
            points.append((frame, x, y))
    
    points.sort(key=lambda t: t[0])
    return points


def load_tracks(output_dir: Path) -> Dict[str, List[Tuple[int, float, float]]]:
    """
    Load all track_*.csv files from the output directory.
    """

    tracks: Dict[str, List[Tuple[int, float, float]]] = {}

    csv_files = sorted(output_dir.glob("track_*.csv"))

    if not csv_files:
        raise FileNotFoundError(f"No track_*.csv files found in {output_dir}")

    for csv_file in csv_files:
        tracks[csv_file.stem] = read_track_csv(csv_file)

    return tracks


def plot_tracks(tracks: Dict[str, List[Tuple[int, float, float]]], save_path: Path) -> None:
    plt.figure(figsize=(10,8))

    for track_name, points in tracks.items():
        xs = [p[1] for p in points]
        ys = [p[2] for p in points]

        plt.plot(xs, ys, marker="o", linewidth=2, label=track_name)

        if points:
            start_frame, start_x, start_y = points[0]
            end_frame, end_x, end_y = points[-1]

            plt.annotate(
                f"{track_name} start",
                (start_x, start_y),
                textcoords="offset points",
                xytext=(5,5)
            )
            plt.annotate(
                f"{track_name} end",
                (end_x, end_y),
                textcoords="offset points",
                xytext=(5, -12)
            )

    plt.title("Track Trajectories")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.grid(True)
    plt.gca().set_aspect("equal", adjustable="datalim")
    plt.tight_layout()

    plt.savefig(save_path, dpi=200)
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