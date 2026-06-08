import subprocess
import time
import csv
import os
from pathlib import Path
from datetime import datetime
 
BINARY     = "./bin/main"
INPUT_DIR  = Path("data/inputs")
OUTPUT_DIR = Path("reports/csv")
SCENARIOS  = ["balanced", "short_burst", "stress", "priority_heavy"]
COUNTS     = [10, 50, 100, 500]
 
def run_simulation(csv_file):
    start  = time.perf_counter()
    result = subprocess.run([BINARY, str(csv_file)], capture_output=True, text=True)
    end    = time.perf_counter()
    return round(end - start, 6), result.stdout, result.returncode
 
def generate_input(scenario, count):
    csv_input = INPUT_DIR / f"bench_{scenario}_n{count}.csv"
    subprocess.run([
        "python3", "scripts/generate_processes.py",
        "--count",      str(count),
        "--scenario",   scenario,
        "--output-dir", str(INPUT_DIR),
        "--seed",       "42",
        "--filename",   f"bench_{scenario}_n{count}",
    ], capture_output=True)
    return csv_input
 
def main():
    if not os.path.exists(BINARY):
        print(f"Error: no se encontro '{BINARY}'. Compila primero con: make")
        return
 
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    INPUT_DIR.mkdir(parents=True, exist_ok=True)
 
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    out_file  = OUTPUT_DIR / f"benchmark_{timestamp}.csv"
    rows      = []
 
    print(f"\n{'Escenario':<18} {'N':>6} {'Tiempo(s)':>12} {'Status':>8}")
    print("-" * 50)
 
    for scenario in SCENARIOS:
        for count in COUNTS:
            csv_input = generate_input(scenario, count)
 
            if not csv_input.exists():
                print(f"{scenario:<18} {count:>6} {'ERROR INPUT':>12} {'FAIL':>8}")
                continue
 
            elapsed, stdout, code = run_simulation(csv_input)
            status = "OK" if code == 0 else "FAIL"
 
            print(f"{scenario:<18} {count:>6} {elapsed:>12.6f} {status:>8}")
            rows.append({
                "scenario":  scenario,
                "count":     count,
                "time_s":    elapsed,
                "status":    status,
                "timestamp": timestamp,
            })
 
    if rows:
        with open(out_file, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=rows[0].keys())
            writer.writeheader()
            writer.writerows(rows)
        print(f"\nResultados guardados en: {out_file}\n")
    else:
        print("\nNo se generaron resultados.\n")
 
if __name__ == "__main__":
    main()