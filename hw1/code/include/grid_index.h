#ifndef GRID_INDEX_H
#define GRID_INDEX_H

#include <vector>
#include <map>
#include "poi.h" 

const double MIN_LAT = 40.4774;
const double MAX_LAT = 45.0159;
const double MIN_LON = -79.7624;
const double MAX_LON = -71.8562;

class GridIndex {
private:
    std::map<std::pair<int, int>, std::vector<POI>> grid;

public:
    GridIndex() = default;
    void build_grid_index(const std::vector<POI>& dataset, double cell_size);
    std::vector<POI> knn_grid_search(const POI& target, int k, double cell_size);
    std::vector<POI> range_grid_search(const POI& target, double radius, double cell_size); 
};

#endif
