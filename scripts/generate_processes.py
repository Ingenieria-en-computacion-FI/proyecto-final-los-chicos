import argparse
import csv
import random
import sys
from pathlib import Path
from datetime import datetime

SCENARIOS = {
    "balanced":       {"burst_time": (1, 20),  "priority": (1, 10), "memory_required": (64,  512)},
    "stress":         {"burst_time": (10, 100), "priority": (1, 5),  "memory_required": (256, 2048)},
    "short_burst":    {"burst_time": (1, 5),    "priority": (1, 10), "memory_required": (16,  128)},
    "priority_heavy": {"burst_time": (1, 30),   "priority": (1, 20), "memory_required": (32,  256)},
}

def generate(count, scenario, seed):
    rng = random.Random(seed)
    cfg = SCENARIOS[scenario]
    processes = []
    for pid in range(1, count + 1):
        bt = rng.randint(*cfg["burst_time"])
        processes.append({
            "pid":             pid,
            "burst_time":      bt,
            "remaining_time":  bt,
            "priority":        rng.randint(*cfg["priority"]),
            "memory_required": rng.randint(*cfg["memory_required"]),
            "state":           "READY",
        })
    return processes

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--count",    "-n", type=int, default=10)
    parser.add_argument("--scenario", "-s", choices=SCENARIOS, default="balanced")
    parser.add_argument("--seed",           type=int, default=None)
    parser.add_argument("--output-dir","-o",type=Path, default=Path("data/inputs"))
    args = parser.parse_args()

    processes = generate(args.count, args.scenario, args.seed)

    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"processes_{args.scenario}_{ts}.csv"
    out = args.output_dir / filename
    out.parent.mkdir(parents=True, exist_ok=True)

    with open(out, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=processes[0].keys())
        writer.writeheader()
        writer.writerows(processes)

    print(f"Generados {args.count} procesos -> {out}")

if __name__ == "__main__":
    main()