import csv
import os
from pathlib import Path
 
INPUT_DIR  = Path("data/inputs")
OUTPUT_DIR = Path("data/outputs")
 
VALID_STATES = {"READY", "RUNNING", "BLOCKED", "FINISHED"}
 
def validate_file(filepath):
    errors   = []
    pids_seen = set()
 
    with open(filepath, newline="") as f:
        reader = csv.DictReader(f)
 
        expected_fields = {"pid", "burst_time", "remaining_time", "priority", "memory_required", "state"}
        if not expected_fields.issubset(set(reader.fieldnames)):
            errors.append(f"Faltan columnas. Esperadas: {expected_fields}")
            return errors
 
        for i, row in enumerate(reader, start=2):
            pid = row["pid"]
 
            if pid in pids_seen:
                errors.append(f"Linea {i}: PID {pid} duplicado")
            pids_seen.add(pid)
 
            if int(row["burst_time"]) <= 0:
                errors.append(f"Linea {i}: burst_time debe ser mayor a 0")
 
            if int(row["remaining_time"]) < 0:
                errors.append(f"Linea {i}: remaining_time no puede ser negativo")
 
            if int(row["remaining_time"]) > int(row["burst_time"]):
                errors.append(f"Linea {i}: remaining_time no puede ser mayor a burst_time")
 
            if int(row["priority"]) <= 0:
                errors.append(f"Linea {i}: priority debe ser mayor a 0")
 
            if int(row["memory_required"]) <= 0:
                errors.append(f"Linea {i}: memory_required debe ser mayor a 0")
 
            if row["state"] not in VALID_STATES:
                errors.append(f"Linea {i}: estado '{row['state']}' no valido. Validos: {VALID_STATES}")
 
    return errors
 
def validate_folder(folder):
    files = sorted(folder.glob("*.csv"))
    if not files:
        print(f"No se encontraron archivos CSV en {folder}")
        return
 
    total_ok   = 0
    total_fail = 0
 
    for filepath in files:
        errors = validate_file(filepath)
        if errors:
            print(f"\nFAIL {filepath.name}")
            for e in errors:
                print(f"     {e}")
            total_fail += 1
        else:
            print(f"OK   {filepath.name}")
            total_ok += 1
 
    print(f"\nResultado: {total_ok} OK  |  {total_fail} con errores")
 
def main():
    print("=== Validando inputs ===")
    validate_folder(INPUT_DIR)
 
    if OUTPUT_DIR.exists():
        print("\n=== Validando outputs ===")
        validate_folder(OUTPUT_DIR)
    else:
        print(f"\nNo existe {OUTPUT_DIR}, omitiendo validacion de outputs.")
 
if __name__ == "__main__":
    main()