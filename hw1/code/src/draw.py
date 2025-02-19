import pandas as pd
import matplotlib.pyplot as plt

knn_df = pd.read_csv("data/knn_results.csv")

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

range_df = pd.read_csv("data/range_results.csv")

plt.figure(figsize=(8, 6))
for N in range_df["N"].unique():
    subset = range_df[range_df["N"] == N]
    plt.plot(subset["r"], subset["time(ms)"], marker='o', label=f"N = {N}")

plt.xlabel("r")
plt.ylabel("Execution Time (ms)")
plt.title("Range Query Execution Time for Different Dataset Sizes")
plt.legend()
plt.grid()
plt.savefig(f"../fig/bf_range.png")
plt.show()
