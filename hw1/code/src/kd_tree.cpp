#include "kd_tree.h"
#include <cmath>
#include <algorithm>
#include <iostream>

KDTree::KDTree(const std::vector<POI>& dataset) {
    std::vector<POI> points = dataset;
    root = build_kd_tree(points, 0, points.size(), 0);
}

KDNode* KDTree::build_kd_tree(std::vector<POI>& points, int start, int end, int depth) {
    if (start >= end) return nullptr;

    int axis = depth % 2;
    size_t median = start + (end - start) / 2;
    std::nth_element(points.begin() + start, points.begin() + median, points.begin() + end,
                     [axis](const POI &a, const POI &b) {
                         return axis == 0 ? a.lat < b.lat : a.lon < b.lon;
                     });

    KDNode* node = new KDNode(points[median]);
    node->left = build_kd_tree(points, start, median, depth + 1);
    node->right = build_kd_tree(points, median + 1, end, depth + 1);

    return node;
}

void KDTree::knn_search_recursive(KDNode* node, const POI& target, int k, int depth, std::priority_queue<std::pair<double, POI>>& max_heap) {
    if (!node) return;

    double dist = euclidean_distance(node->point, target);

    if (node->point.id != target.id) {
        if ((int)max_heap.size() < k || dist < max_heap.top().first) {
            max_heap.push({dist, node->point});
            if ((int)max_heap.size() > k) {
                max_heap.pop();
            }
        }
    }

    int axis = depth % 2;
    double diff = (axis == 0) ? (target.lat - node->point.lat) : (target.lon - node->point.lon);
    bool go_left = diff < 0; 

    knn_search_recursive(go_left ? node->left : node->right, target, k, depth + 1, max_heap);

    if ((int)max_heap.size() < k || diff * diff < max_heap.top().first) {
        knn_search_recursive(go_left ? node->right : node->left, target, k, depth + 1, max_heap);
    }

}

std::vector<POI> KDTree::knn_search(const POI& target, int k) {
    std::priority_queue<std::pair<double, POI>> max_heap;
    knn_search_recursive(root, target, k, 0, max_heap);

    std::vector<POI> results;
    while (!max_heap.empty()) {
        results.push_back(max_heap.top().second);
        max_heap.pop();
    }
    std::reverse(results.begin(), results.end());
    return results;
}

void KDTree::range_search_recursive(KDNode* node, const POI& target, double radius, int depth, std::vector<POI>& results) {
    if (!node) return;

    if (node->point.id != target.id) {
        if (euclidean_distance(node->point, target) <= radius) {
            results.push_back(node->point);
        }
    }

    int axis = depth % 2;
    bool go_left = (axis == 0) ? target.lat < node->point.lat : target.lon < node->point.lon;

    range_search_recursive(go_left ? node->left : node->right, target, radius, depth + 1, results);

    if (std::fabs((axis == 0 ? target.lat - node->point.lat : target.lon - node->point.lon)) <= radius) {
        range_search_recursive(go_left ? node->right : node->left, target, radius, depth + 1, results);
    }
}

std::vector<POI> KDTree::range_search(const POI& target, double radius) {
    std::vector<POI> results;
    range_search_recursive(root, target, radius, 0, results);
    return results;
}

void KDTree::delete_tree(KDNode* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}

KDTree::~KDTree() {
    delete_tree(root);
}
