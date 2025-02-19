import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

# 数据路径
data_path = "../data/"

# 读取 brute force 结果 (knn_results.csv)
bf_knn_file = os.path.join(data_path, "range_results.csv")
bf_knn_df = pd.read_csv(bf_knn_file)

# 只筛选出 N = 1630238 的数据
bf_knn_df = bf_knn_df[bf_knn_df["N"] == 1630237]

# 获取所有 Grid Index 相关的 kNN 结果文件
grid_knn_files = glob.glob(os.path.join(data_path, "grid_range_results_*.csv"))

plt.figure(figsize=(8, 6))

# 绘制 brute force 结果曲线
plt.plot(bf_knn_df["r"], bf_knn_df["time(ms)"], marker='o', linestyle='-', label="Brute Force")

# 读取 brute force 结果 (knn_results.csv)
kd_knn_file = os.path.join(data_path, "kd_range_results.csv")
kd_knn_df = pd.read_csv(kd_knn_file)

# 只筛选出 N = 1630238 的数据
kd_knn_df = kd_knn_df[kd_knn_df["N"] == 1630238]

# 绘制 brute force 结果曲线
plt.plot(kd_knn_df["r"], kd_knn_df["time(ms)"], marker='o', linestyle='-', label="KD-Tree")

# 处理每个 grid index 结果
for file in grid_knn_files:
    cell_size = file.split("_")[-1].replace(".csv", "")  # 提取 cell_size
    cell_size = "{:.2f}".format(float(cell_size))  # 只保留小数点后两位
    
    grid_knn_df = pd.read_csv(file)
    grid_knn_df = grid_knn_df[grid_knn_df["N"] == 1630238]  # 筛选 N=1630238

    if not grid_knn_df.empty:
        plt.plot(grid_knn_df["r"], grid_knn_df["time(ms)"], marker='o', linestyle='-', label=f"Grid (cell_size={cell_size})")

# 设置图例和标题
plt.xlabel("r")
plt.ylabel("Execution Time (ms)")
plt.title("Range Query Execution Time Comparison (N=1630238)")
plt.legend()
plt.grid()
plt.savefig(f"../fig/range_bfvsgrid.png")
plt.show()
