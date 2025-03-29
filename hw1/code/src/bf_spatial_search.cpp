#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <map>

#define MAX_DATASET_SIZE 1630238

struct POI {
    long long id;
    double lat, lon;
};

double euclidean_distance(const POI &p1, const POI &p2) {
    return sqrt(pow(p1.lat - p2.lat, 2) + pow(p1.lon - p2.lon, 2));
}

std::vector<POI> read_csv(const std::string &filename) {
    std::vector<POI> dataset;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        return dataset;
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id_str, lat_str, lon_str, name;
        std::getline(ss, id_str, ',');
        std::getline(ss, lat_str, ',');
        std::getline(ss, lon_str, ',');
        std::getline(ss, name);

        if (!id_str.empty() && !lat_str.empty() && !lon_str.empty()) {
            try {
                long long id = static_cast<long long>(std::stoull(id_str));
                double lat = std::stod(lat_str);
                double lon = std::stod(lon_str);

                dataset.push_back({id, lat, lon});
            } catch (const std::exception &e) {
                std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }

    file.close();
    return dataset;
}

std::vector<std::pair<POI, double>> knn_linear_search(const std::vector<POI> &dataset, long long target_id, int k) {
    std::vector<std::pair<POI, double>> distances;
    POI target;
    bool found = false;

    for (const auto &poi : dataset) {
        if (poi.id == target_id) {
            target = poi;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Error: Target ID " << target_id << " not found in dataset.\n";
        return {};
    }

    for (const auto &poi : dataset) {
        if (poi.id != target_id) {
            double dist = euclidean_distance(target, poi);
            distances.emplace_back(poi, dist);
        }
    }

    if ((int)distances.size() > k) {
        std::partial_sort(distances.begin(), distances.begin() + k, distances.end(),
                          [](const auto &a, const auto &b) {
                              return a.second < b.second;
                          });
        distances.resize(k);
    } else {
        std::sort(distances.begin(), distances.end(),
                  [](const auto &a, const auto &b) {
                      return a.second < b.second;
                  });
    }

    return distances;
}


std::vector<POI> range_query_linear_search(const std::vector<POI> &dataset, int target_id, double radius) {
    std::vector<POI> results;
    POI target;

    for (const auto &poi : dataset) {
        if (poi.id == target_id) {
            target = poi;
            break;
        }
    }

    for (const auto &poi : dataset) {
        if (poi.id != target_id && euclidean_distance(target, poi) <= radius) {
            results.push_back(poi);
        }
    }

    return results;
}

void run_tests(const std::vector<POI> &dataset) {
    std::vector<int> k_values = {1, 5, 10, 50, 100, 500};
    std::vector<double> r_values = {0.01, 0.05, 0.1, 0.2, 0.5};
    std::vector<int> N_values = {1000, 10000, 100000, MAX_DATASET_SIZE};
    
    std::map<int, std::vector<double>> knn_times;
    std::map<int, std::vector<double>> range_times;

    for (int N : N_values) {
        std::vector<POI> subset(dataset.begin(), dataset.begin() + N);
        long long target_id = subset[N / 2].id;

        std::vector<double> times_knn, times_range;

        for (int k : k_values) {
            auto start = std::chrono::high_resolution_clock::now();
            auto ans = knn_linear_search(subset, target_id, k);
            auto end = std::chrono::high_resolution_clock::now();
            double duration_knn = std::chrono::duration<double, std::milli>(end - start).count();
            times_knn.push_back(duration_knn);
            if (k == 5 && N == MAX_DATASET_SIZE) {
                // std::cout << subset[N / 2].lat << "," << subset[N / 2].lon << std::endl;
                std::cout << "kNN results of " << target_id << " for BF (k = " << k << ", N = " << N << "): ";
                for (const auto &pair : ans) {
                    std::cout << pair.first.id << " ";
                }
                std::cout << std::endl;
            }
        }
        knn_times[N] = times_knn;

        for (double r : r_values) {
            auto start = std::chrono::high_resolution_clock::now();
            auto ans = range_query_linear_search(subset, target_id, r);
            auto end = std::chrono::high_resolution_clock::now();
            double duration_range = std::chrono::duration<double, std::milli>(end - start).count();
            times_range.push_back(duration_range);
            if (r == 0.01 && N == 100000) {
                std::cout << "Range Query results of " << target_id << " for BF (r = " << r << ", N = " << N << "): ";
                for (const auto &poi : ans) {
                    std::cout << poi.id << " ";
                }
                std::cout << std::endl;
            }
            
        }
        range_times[N] = times_range;
    }

    std::ofstream knn_file("data/knn_results.csv");
    knn_file << "N,k,time(ms)\n";
    for (const auto &entry : knn_times) {
        int N = entry.first;
        for (size_t i = 0; i < k_values.size(); ++i) {
            knn_file << N << "," << k_values[i] << "," << entry.second[i] << "\n";
        }
    }
    knn_file.close();

    std::ofstream range_file("data/range_results.csv");
    range_file << "N,r,time(ms)\n";
    for (const auto &entry : range_times) {
        int N = entry.first;
        for (size_t i = 0; i < r_values.size(); ++i) {
            range_file << N << "," << r_values[i] << "," << entry.second[i] << "\n";
        }
    }
    range_file.close();
}


int main() {
    std::string filename = "data/cleaned_data.csv";
    std::vector<POI> dataset = read_csv(filename);

    if (dataset.empty()) {
        return 1;
    }

    run_tests(dataset);
    return 0;
}
