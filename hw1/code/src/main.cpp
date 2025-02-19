#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "grid_index.h"
#include "kd_tree.h"

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
            dataset.push_back({std::stoll(id_str), std::stod(lat_str), std::stod(lon_str)});
        }
    }

    file.close();
    return dataset;
}

void knn_save_results_to_csv(const std::string& filename, const std::vector<std::tuple<int, double, double>>& results) {
    std::ofstream file(filename);
    file << "N,k,time(ms)\n";
    for (const auto& [N, param, time] : results) {
        file << N << "," << param << "," << time << "\n";
    }
    file.close();
}

void range_save_results_to_csv(const std::string& filename, const std::vector<std::tuple<int, double, double>>& results) {
    std::ofstream file(filename);
    file << "N,r,time(ms)\n";
    for (const auto& [N, param, time] : results) {
        file << N << "," << param << "," << time << "\n";
    }
    file.close();
}

// 运行测试
void run_tests(const std::vector<POI> &dataset) {
    std::vector<int> k_values = {1, 5, 10, 50, 100, 500};
    std::vector<double> r_values = {0.01, 0.05, 0.1, 0.2, 0.5};
    std::vector<int> N_values = {1000, 10000, 100000, (int)dataset.size()};
    std::vector<double> cell_sizes = {0.01, 0.05, 0.1, 0.2};

    std::vector<std::tuple<int, double, double>> kd_knn_results, kd_range_results;

    for (double cell_size : cell_sizes) {
        
        // GridIndex grid;
        // grid.build_grid_index(dataset, cell_size);

        std::vector<std::tuple<int, double, double>> grid_knn_results, grid_range_results;

        for (int N : N_values) {
            std::vector<POI> subset(dataset.begin(), dataset.begin() + N);
            int target_id = subset[N / 2].id;

            GridIndex grid;
            grid.build_grid_index(subset, cell_size);

            for (int k : k_values) {
                auto start = std::chrono::high_resolution_clock::now();
                std::cout << "k size is " << k << " subset size is " << subset.size() << " cell size is " << cell_size << std::endl;
                grid.knn_grid_search(subset[N / 2], k, cell_size);
                auto end = std::chrono::high_resolution_clock::now();
                double grid_time = std::chrono::duration<double, std::milli>(end - start).count();
                grid_knn_results.emplace_back(N, k, grid_time);
            }

            for (double r : r_values) {
                auto start = std::chrono::high_resolution_clock::now();
                grid.range_grid_search(subset[N / 2], r, cell_size);
                auto end = std::chrono::high_resolution_clock::now();
                double grid_time = std::chrono::duration<double, std::milli>(end - start).count();
                grid_range_results.emplace_back(N, r, grid_time);
            }
        }

        knn_save_results_to_csv("data/grid_knn_results_" + std::to_string(cell_size) + ".csv", grid_knn_results);
        range_save_results_to_csv("data/grid_range_results_" + std::to_string(cell_size) + ".csv", grid_range_results);
    }

    // KDTree kd_tree(dataset);

    for (int N : N_values) {
        if ((int)dataset.size() < N) break;
        
        std::vector<POI> subset(dataset.begin(), dataset.begin() + N);
        int target_id = subset[N / 2].id;

        KDTree kd_tree(subset);

        for (int k : k_values) {
            auto start = std::chrono::high_resolution_clock::now();
            kd_tree.knn_search(subset[N / 2], k);
            auto end = std::chrono::high_resolution_clock::now();
            double kd_time = std::chrono::duration<double, std::milli>(end - start).count();
            kd_knn_results.emplace_back(N, k, kd_time);
        }

        for (double r : r_values) {
            auto start = std::chrono::high_resolution_clock::now();
            kd_tree.range_search(subset[N / 2], r);
            auto end = std::chrono::high_resolution_clock::now();
            double kd_time = std::chrono::duration<double, std::milli>(end - start).count();
            kd_range_results.emplace_back(N, r, kd_time);
        }
    }

    knn_save_results_to_csv("data/kd_knn_results.csv", kd_knn_results);
    range_save_results_to_csv("data/kd_range_results.csv", kd_range_results);
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