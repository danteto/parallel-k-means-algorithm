This project describes a parallelization of k-means-algorithm

Introduction

The problem is identifying clusters of data objects in a dataset. A cluster is a collection of objects where these objects are similar and dissimilar to the other cluster.
Clusters process of organizing objects into groups such that data points in the same groups are similar to the data points in the same group. There are many types of clustering. In this work, k-means clustering is used. K-means is one of the simplest algorithms that solve the clustering problem. The goal of this algorithm is to find groups in the data, with the number of groups represented by the variable K.
The application takes as input the coordinates of k initial centroids and a set of data points. K-Means conducts an iterative process, where points are re-clustered according to the minimum Euclidean distance between them and the centroids. After that, the centroid of each partition is recalculated taking the mean of all points in the partition, and the process is repeated until no centroid is changed and no points are assigned to another cluster. In the end, the algorithm should return the coordinates of the final k centroids.
The goal of the work is to parallelize the given sequential implementation.


Time complexity/performance analysis

The running time of the algorithm is O(T * K * N * D) assuming that
T - number of iterations needed to complete the clustering process.  
K - number of clusters (centroids)
N - number of all points, that should be clustered
D - number of dimensions
In our case D = 3, so the time complexity of the algorithm is O(T * N * K)