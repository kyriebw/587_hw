#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <regex>

using namespace std;

struct POI {
    string id;
    string lat;
    string lon;
    string name;
};

bool isValidNumber(const string& str) {
    return regex_match(str, regex("^-?\\d+(\\.\\d+)?$"));
}

bool isValidID(const string& str) {
    return regex_match(str, regex("^\\d+$"));
}

void cleanCSV(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    unordered_set<string> uniqueIDs;
    vector<POI> validEntries;

    if (getline(inFile, line)) {
        outFile << line << endl;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        string id, lat, lon, name;

        getline(ss, id, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, name);

        if (id.empty() || lat.empty() || lon.empty()) {
            std::cerr << "数据缺失: " << line << std::endl;
            continue;
        }

        if (!isValidID(id) || !isValidNumber(lat) || !isValidNumber(lon)) {
            std::cerr << "格式错误: " << line << std::endl;
            continue;
        }

        if (uniqueIDs.find(id) != uniqueIDs.end()) {
            std::cerr << "重复 ID: " << id << " -> " << line << std::endl;
            continue;
        }

        POI poi = {id, lat, lon, name};
        validEntries.push_back(poi);
        uniqueIDs.insert(id);
    }

    for (const auto& poi : validEntries) {
        outFile << poi.id << "," << poi.lat << "," << poi.lon << "," << poi.name << endl;
    }

    cout << "Data cleaning complete. Output saved to: " << outputFile << endl;

    inFile.close();
    outFile.close();
}

int main() {
    string inputFile = "/Users/bowen/Desktop/587/587_hw/hw1/code/data/interpreter.csv"; 
    string outputFile = "/Users/bowen/Desktop/587/587_hw/hw1/code/data/cleaned_data.csv";
    cleanCSV(inputFile, outputFile);
    return 0;
}
