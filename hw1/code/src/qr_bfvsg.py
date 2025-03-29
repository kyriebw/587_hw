import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

data_path = "../data/"

bf_knn_file = os.path.join(data_path, "range_results.csv")
bf_knn_df = pd.read_csv(bf_knn_file)

bf_knn_df = bf_knn_df[bf_knn_df["N"] == 1630238]

grid_knn_files = glob.glob(os.path.join(data_path, "grid_range_results_*.csv"))

plt.figure(figsize=(8, 6))

plt.plot(bf_knn_df["r"], bf_knn_df["time(ms)"], marker='o', linestyle='-', label="Brute Force")

kd_knn_file = os.path.join(data_path, "kd_range_results.csv")
kd_knn_df = pd.read_csv(kd_knn_file)

kd_knn_df = kd_knn_df[kd_knn_df["N"] == 1630238]

plt.plot(kd_knn_df["r"], kd_knn_df["time(ms)"], marker='o', linestyle='-', label="KD-Tree")

for file in grid_knn_files:
    cell_size = file.split("_")[-1].replace(".csv", "")
    cell_size = "{:.2f}".format(float(cell_size))
    
    grid_knn_df = pd.read_csv(file)
    grid_knn_df = grid_knn_df[grid_knn_df["N"] == 1630238]

    if not grid_knn_df.empty:
        plt.plot(grid_knn_df["r"], grid_knn_df["time(ms)"], marker='o', linestyle='-', label=f"Grid (cell_size={cell_size})")

plt.xlabel("r")
plt.ylabel("Execution Time (ms)")
plt.title("Range Query Execution Time Comparison (N=1630238)")
plt.legend()
plt.grid()
plt.savefig(f"../fig/range_bfvsgrid.png")
plt.show()
