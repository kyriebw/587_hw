#ifndef KD_TREE_H
#define KD_TREE_H

#include <vector>
#include <queue>
#include "poi.h"

struct KDNode {
    POI point;
    KDNode* left;
    KDNode* right;
    
    KDNode(POI p) : point(p), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDNode* root;
    explicit KDTree(const std::vector<POI>& dataset);
    ~KDTree();

    std::vector<POI> knn_search(const POI& target, int k);
    std::vector<POI> range_search(const POI& target, double radius);

private:
    KDNode* build_kd_tree(std::vector<POI>& points, int start, int end, int depth);
    void knn_search_recursive(KDNode* node, const POI& target, int k, int depth, std::priority_queue<std::pair<double, POI>>& max_heap);
    void range_search_recursive(KDNode* node, const POI& target, double radius, int depth, std::vector<POI>& results);
    void delete_tree(KDNode* node);
};

#endif