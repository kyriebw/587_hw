# HW1 Results

| Name       | Email            | USCID      |
| ---------- | ---------------- | ---------- |
| Bowen Wang | bwang443@usc.edu | 1454229482 |

## 1. Dataset

My dataset contains 1630238 data in total, and using `src/data_clean.cpp` for data cleaning.

## 2. Brute Force

### 2.1 KNN

The plot is below:
                 ![image-20250218162632582](/Users/bowen/Library/Application Support/typora-user-images/image-20250218162632582.png)       .      

As we can see, when `N` is larger, the total execution time should be larger, since brute force always goes over all the data in the dataset. However, when `k` increases, the execution time does not change significantly, as the algorithm still needs to traverse almost the entire dataset in most cases. For fluctuation at the begin, I think it should be that when `k` is very small, `std::nth_element()` still needs to scan the entire array (O(N)); but when `k` is larger, `nth_element()` only needs to adjust a smaller portion, reducing unnecessary scans and improving query time.

### 2.2 Range Query

Here is the plot:

![image-20250218163311092](/Users/bowen/Library/Application Support/typora-user-images/image-20250218163311092.png)

Same as above, when `N` is larger, the total execution time should be larger, since brute force always goes over all the data in the dataset. However, when `k` increases, the execution time does not change significantly, as the algorithm still needs to traverse almost the entire dataset in most cases.

### 2.3 Time between kNN and Range Query 

I notice that the time used in range query is less than knn, and I think that in brute-force knn, all data points must be scanned to compute distances, followed by `nth_element()` to find the k-nearest neighbors, leading to extra selection or sorting overhead (O(N log k)). In contrast, range query only checks if each point is within a given radius (O(N)), avoiding sorting, which makes it generally faster.

### 2.4 Results

Here are the results of kNN and Range Query. Firstly, I set r = 0.05, N = 100000 for testing and validation.

![image-20250219172044716](/Users/bowen/Library/Application Support/typora-user-images/image-20250219172044716.png)

But the number of output is still a lot, so I change r into 0.01 for range query, and set k = 5, N = 1630238 for kNN we will use these two results to compare with other methods.
![image-20250219174023331](/Users/bowen/Library/Application Support/typora-user-images/image-20250219174023331.png)

## 3. Spatial Indexes

### 3.1 Grid Index

#### 3.1.1 kNN

![image-20250218165800755](/Users/bowen/Library/Application Support/typora-user-images/image-20250218165800755.png)

There are four plots with different cell sizes for kNN, and we notice that for some cell sizes, when `N` is 1000 and `k` is 100 or 500, the execution time is way larger than others. From my opinion, I think the reason of this is when cell size is not huge and the data is sparse, there will be a lot of cells, which can be larger than the size of dataset, e.g. 1000, 10000. And at the same time, there will also be a lot of empty cells that the algorithm should retrieve, which will lead to the high execution time. I print the search radius of knn, and we can see that there are some scenarios that have large search radius:

When dataset size is 1000, and cell size is 0.01, if we try to find top-100 NN, there will be 309 as search radius, which means we need to go over 309 * 309 = 95481 cells, and if we try to find top-500, the number of cells we need to go over is 720 * 720 = 518400

​                                  ![image-20250218172736202](/Users/bowen/Library/Application Support/typora-user-images/image-20250218172736202.png) 

The thing could also happen in other dataset size and cell size, but especially in sparse ones:

![image-20250218173155877](/Users/bowen/Library/Application Support/typora-user-images/image-20250218173155877.png)

So in the cases above, I would like to only see the results for dense dataset, which is the whole dataset with the size of 1630238, and compare that to the brute force one.

#### 3.1.2 Whole dataset for kNN

![image-20250218174439571](/Users/bowen/Library/Application Support/typora-user-images/image-20250218174439571.png)

Above is the comparison between grid index with different cell size and brute force one, we can see that in large and not too sparse dataset, the grid index is more efficient than brute force, and small cell size could give us better performance, since it may could help us refine.

#### 3.1.3 Range Query

![image-20250218165813856](/Users/bowen/Library/Application Support/typora-user-images/image-20250218165813856.png)

As we can see, for all experiments in the same cell size, when `N`  grows up, the execution time increases as well; but for the different cell sizes, there is no significant time difference between them when `N` and `r` are set, I think it could be that when radius is set, the range will also be set, and the number of grids are not that huge, so go over all of them will not take too much time, which makes the final execution time looks same for different cell sizes.

#### 3.1.4 Whole dataset for Range Query

![image-20250218222719082](/Users/bowen/Library/Application Support/typora-user-images/image-20250218222719082.png)

I also picked the results of different methods as shown above when `N` is 1630238, which is the whole dataset, we can see that grid index time is always less than brute force, that's because grid index will not go over all of the data.

#### 3.1.5 Results

KNN and Range Query's results when cell size is 0.01
![image-20250219175115530](/Users/bowen/Library/Application Support/typora-user-images/image-20250219175115530.png)

KNN and Range Query's results when cell size is 0.05

![image-20250219175132799](/Users/bowen/Library/Application Support/typora-user-images/image-20250219175132799.png)

KNN and Range Query's results when cell size is 0.10

![image-20250219175255827](/Users/bowen/Library/Application Support/typora-user-images/image-20250219175255827.png)

KNN and Range Query's results when cell size is 0.20

![image-20250219175312974](/Users/bowen/Library/Application Support/typora-user-images/image-20250219175312974.png)

As we can see, they are all same with Brute Force, so the results are right

### 3.2 KD-Tree

#### 3.2.1 kNN

![image-20250218225028258](/Users/bowen/Library/Application Support/typora-user-images/image-20250218225028258.png)

For KD-Tree, when `k` or `N` grows larger, the execution time will also increase, that's because we need search more on the binary tree or the tree structure could be more complex

#### 3.2.2 Whole dataset for kNN

![image-20250218230812380](/Users/bowen/Library/Application Support/typora-user-images/image-20250218230812380.png)

I compared different methods for kNN, we can see that KD-Tree only worse than grid index with fine granularity

#### 3.2.3 Range Query

![image-20250218231034520](/Users/bowen/Library/Application Support/typora-user-images/image-20250218231034520.png)

We can see that the execution time increase with `N` and `r`, same reason as kNN.

#### 3.2.4 Whole dataset for Range Query

![image-20250218231332754](/Users/bowen/Library/Application Support/typora-user-images/image-20250218231332754.png)

We can see that the KD-Tree works very well on range query, better than all of others.

#### 3.2.5 Results

![image-20250219180047878](/Users/bowen/Library/Application Support/typora-user-images/image-20250219180047878.png)

Here are the results of KD-Tree, and we can see that the answer are still same with Brute Force and Grid Index, so it should be right.