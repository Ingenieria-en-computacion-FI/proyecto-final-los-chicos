import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
 
INPUT_DIR  = Path("reports/csv")
OUTPUT_DIR = Path("reports/png")
 
def load_latest_benchmark():
    files = sorted(INPUT_DIR.glob("benchmark_*.csv"))
    if not files:
        print("Error: no se encontro ningun archivo benchmark en reports/csv/")
        return None
    return pd.read_csv(files[-1])
 
def grafica_tiempo_por_escenario(df):
    fig, ax = plt.subplots(figsize=(10, 5))
    for scenario in df["scenario"].unique():
        subset = df[df["scenario"] == scenario]
        ax.plot(subset["count"], subset["time_s"], marker="o", label=scenario)
    ax.set_title("Tiempo de ejecucion por escenario")
    ax.set_xlabel("Numero de procesos")
    ax.set_ylabel("Tiempo (segundos)")
    ax.legend()
    ax.grid(True)
    out = OUTPUT_DIR / "tiempo_por_escenario.png"
    plt.tight_layout()
    plt.savefig(out)
    plt.close()
    print(f"Guardada: {out}")
 
def grafica_comparacion_n(df):
    counts = df["count"].unique()
    fig, axes = plt.subplots(1, len(counts), figsize=(14, 5), sharey=True)
    for ax, count in zip(axes, counts):
        subset = df[df["count"] == count]
        ax.bar(subset["scenario"], subset["time_s"])
        ax.set_title(f"N = {count}")
        ax.set_xlabel("Escenario")
        ax.tick_params(axis="x", rotation=20)
    axes[0].set_ylabel("Tiempo (segundos)")
    fig.suptitle("Comparacion de tiempos por numero de procesos")
    plt.tight_layout()
    out = OUTPUT_DIR / "comparacion_por_n.png"
    plt.savefig(out)
    plt.close()
    print(f"Guardada: {out}")
 
def grafica_status(df):
    status_counts = df["status"].value_counts()
    fig, ax = plt.subplots(figsize=(5, 5))
    ax.pie(status_counts, labels=status_counts.index, autopct="%1.1f%%", startangle=90)
    ax.set_title("Resultado de simulaciones (OK vs FAIL)")
    out = OUTPUT_DIR / "status.png"
    plt.tight_layout()
    plt.savefig(out)
    plt.close()
    print(f"Guardada: {out}")
 
def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
 
    df = load_latest_benchmark()
    if df is None:
        return
 
    print(f"Leyendo: {sorted(INPUT_DIR.glob('benchmark_*.csv'))[-1]}\n")
 
    grafica_tiempo_por_escenario(df)
    grafica_comparacion_n(df)
    grafica_status(df)
 
    print("\nTodas las graficas guardadas en reports/png/")
 
if __name__ == "__main__":
    main()
 