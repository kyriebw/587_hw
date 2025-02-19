#include "grid_index.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <set>

bool is_within_bounds(double lat, double lon) {
    return lat >= MIN_LAT && lat <= MAX_LAT && lon >= MIN_LON && lon <= MAX_LON;
}

void GridIndex::build_grid_index(const std::vector<POI>& dataset, double cell_size) {
    grid.clear();

    for (const auto& poi : dataset) {
        if (!is_within_bounds(poi.lat, poi.lon)) {
            continue;
        }

        int cell_x = static_cast<int>(std::floor(poi.lat / cell_size));
        int cell_y = static_cast<int>(std::floor(poi.lon / cell_size));
        grid[{cell_x, cell_y}].push_back(poi);
    }

    // std::cout << "dataset size is " << dataset.size() << std::endl;
}

std::vector<POI> GridIndex::knn_grid_search(const POI& target, int k, double cell_size) {
    if (!is_within_bounds(target.lat, target.lon)) {
        return {};
    }

    int cell_x = static_cast<int>(std::floor(target.lat / cell_size));
    int cell_y = static_cast<int>(std::floor(target.lon / cell_size));

    std::vector<POI> candidates;
    int search_radius = 1;
    bool enough_neighbors = false;

    // **记录已访问的 cell，避免重复计算**
    std::set<std::pair<int, int>> visited_cells;

    while (!enough_neighbors) {
        for (int dx = -search_radius; dx <= search_radius; ++dx) {
            for (int dy = -search_radius; dy <= search_radius; ++dy) {
                int new_x = cell_x + dx;
                int new_y = cell_y + dy;

                // **如果已访问，跳过**
                if (visited_cells.find({new_x, new_y}) != visited_cells.end()) continue;
                visited_cells.insert({new_x, new_y});

                auto it = grid.find({new_x, new_y});
                if (it != grid.end()) {
                    candidates.insert(candidates.end(), it->second.begin(), it->second.end());
                }
            }
        }

        if ((int)candidates.size() >= k) {
            enough_neighbors = true;
        } else {
            search_radius++;
        }
    }

    std::cout << "Checked " << candidates.size() << " candidate points for kNN query, in grid size " << grid.size() << " with search radius " << search_radius << std::endl;

    // **计算所有候选点的距离**
    std::vector<std::pair<double, POI>> distances;
    for (const auto& poi : candidates) {
        double dist = euclidean_distance(poi, target);
        distances.emplace_back(dist, poi);
    }

    // **使用 std::nth_element() 找到前 k 个最近点**
    if ((int)distances.size() > k) {
        std::nth_element(distances.begin(), distances.begin() + k, distances.end(),
                         [](const auto &a, const auto &b) {
                             return a.first < b.first;  // 按距离排序
                         });
        distances.resize(k);  // 仅保留前 k 个
    }

    // **提取 POI 结果**
    std::vector<POI> knn_results;
    for (const auto& p : distances) {
        knn_results.push_back(p.second);
    }

    return knn_results;
}


std::vector<POI> GridIndex::range_grid_search(const POI& target, double radius, double cell_size) {
    if (!is_within_bounds(target.lat, target.lon)) {
        std::cerr << "Error: Target point is out of the defined geographic range!\n";
        return {};
    }

    int min_x = static_cast<int>(std::floor((target.lat - radius) / cell_size));
    int max_x = static_cast<int>(std::ceil((target.lat + radius) / cell_size));
    int min_y = static_cast<int>(std::floor((target.lon - radius) / cell_size));
    int max_y = static_cast<int>(std::ceil((target.lon + radius) / cell_size));


    std::vector<POI> results;

    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            auto it = grid.find({x, y});
            if (it != grid.end()) {
                for (const POI& poi : it->second) {
                    double dist = euclidean_distance(poi, target);
                    if (dist <= radius) {
                        results.push_back(poi);
                    }
                }
            }
        }
    }

    return results;
}
