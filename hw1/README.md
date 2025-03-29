

# HW1 README

| Name       | Email            | USCID      |
| ---------- | ---------------- | ---------- |
| Bowen Wang | bwang443@usc.edu | 1454229482 |

I used Clang to compile my homework, which was primarily completed using C++17, with Python utilized for plotting.

1. First, we should make sure your python environment has `pandas` and `matplotlib`:

   ```bash
   cd code
   pip install -r requirements.txt
   ```

2. Then we go over the data clean

   First we should have your own data in `data` folder, named `interpreter.csv`, then:

   ```bash
   cd build
   rm -rf *
   cmake ..
   make dc       # which means data clean
   ```

3. Next, let's do the brute force one

   ```bash
   make bf       # which means brute force
   ```

   After that, we will have two files name `knn_results.csv` and `range_results.csv` in data folder, who store the time consumed by using different N, k, r

4. Then we do the grid index and kd-tree index

   ```bash
   make run
   ```

   We will have 10 files, 4 for grid index's knn with different cell sizes, 4 for grid index's rq with different cell sizes, and other 2 for kd-tree's knn and rq

5. Once we finished step 4 and 5, we can plot the figures:

   ```bash
   cd ../src   # jump from build to src
   python draw_3rd.py
   ```

   Finally you will have figures in `fig` folder.

6. My results are in `data` and `fig` folder