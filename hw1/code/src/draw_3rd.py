import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

knn_df = pd.read_csv("../data/knn_results.csv")

plt.figure(figsize=(8, 6))
for N in knn_df["N"].unique():
    subset = knn_df[knn_df["N"] == N]
    plt.plot(subset["k"], subset["time(ms)"], marker='o', label=f"N = {N}")

plt.xlabel("k")
plt.ylabel("Execution Time (ms)")
plt.title("kNN Execution Time for Different Dataset Sizes")
plt.legend()
plt.grid()
plt.savefig(f"../fig/bf_knn.png")
plt.show()

range_df = pd.read_csv("../data/range_results.csv")

plt.figure(figsize=(8, 6))
for N in range_df["N"].unique():
    subset = range_df[range_df["N"] == N]
    plt.plot(subset["r"], subset["time(ms)"], marker='o', label=f"N = {N}")

plt.xlabel("r")
plt.ylabel("Execution Time (ms)")
plt.title("Range Query Execution Time for Different Dataset Sizes")
plt.legend()
plt.grid()
plt.savefig(f"../fig/bf_rq.png")
plt.show()

data_path = "../data/"

grid_knn_files = glob.glob(os.path.join(data_path, "grid_knn_results_*.csv"))
print(grid_knn_files)
grid_range_files = glob.glob(os.path.join(data_path, "grid_range_results_*.csv"))
kd_knn_file = os.path.join(data_path, "kd_knn_results.csv")
kd_range_file = os.path.join(data_path, "kd_range_results.csv")

for file in grid_knn_files:
    print(file)
    knn_df = pd.read_csv(file)
    cell_size = "{:.2f}".format(float(file.split("_")[-1].replace(".csv", "")))

    plt.figure(figsize=(8, 6))
    for N in knn_df["N"].unique():
        subset = knn_df[knn_df["N"] == N]
        plt.plot(subset["k"], subset["time(ms)"], marker='o', label=f"N = {N}")

    plt.xlabel("k")
    plt.ylabel("Execution Time (ms)")
    plt.title(f"Grid Index kNN Execution Time (Cell Size = {cell_size})")
    plt.legend()
    plt.grid()
    plt.savefig(f"../fig/grid_knn_{cell_size}.png")
    plt.show()

for file in grid_range_files:
    range_df = pd.read_csv(file)
    cell_size = "{:.2f}".format(float(file.split("_")[-1].replace(".csv", "")))

    plt.figure(figsize=(8, 6))
    for N in range_df["N"].unique():
        subset = range_df[range_df["N"] == N]
        plt.plot(subset["r"], subset["time(ms)"], marker='o', label=f"N = {N}")

    plt.xlabel("r")
    plt.ylabel("Execution Time (ms)")
    plt.title(f"Grid Index Range Query Execution Time (Cell Size = {cell_size})")
    plt.legend()
    plt.grid()
    plt.savefig(f"../fig/grid_range_{cell_size}.png")
    plt.show()

if os.path.exists(kd_knn_file):
    kd_knn_df = pd.read_csv(kd_knn_file)

    plt.figure(figsize=(8, 6))
    for N in kd_knn_df["N"].unique():
        subset = kd_knn_df[kd_knn_df["N"] == N]
        plt.plot(subset["k"], subset["time(ms)"], marker='o', label=f"N = {N}")

    plt.xlabel("k")
    plt.ylabel("Execution Time (ms)")
    plt.title("KD-Tree kNN Execution Time")
    plt.legend()
    plt.grid()
    plt.savefig("../fig/kd_knn.png")
    plt.show()

if os.path.exists(kd_range_file):
    kd_range_df = pd.read_csv(kd_range_file)

    plt.figure(figsize=(8, 6))
    for N in kd_range_df["N"].unique():
        subset = kd_range_df[kd_range_df["N"] == N]
        plt.plot(subset["r"], subset["time(ms)"], marker='o', label=f"N = {N}")

    plt.xlabel("r")
    plt.ylabel("Execution Time (ms)")
    plt.title("KD-Tree Range Query Execution Time")
    plt.legend()
    plt.grid()
    plt.savefig("../fig/kd_range.png")
    plt.show()
