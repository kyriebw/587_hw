#ifndef POI_H
#define POI_H

#include <string>
#include <tuple>

struct POI {
    long long id;
    double lat, lon;

    bool operator<(const POI& other) const {
        return std::tie(lat, lon, id) < std::tie(other.lat, other.lon, other.id);
    }
};

inline double euclidean_distance(const POI& a, const POI& b) {
    return std::sqrt(std::pow(a.lat - b.lat, 2) + std::pow(a.lon - b.lon, 2));
}

#endif
