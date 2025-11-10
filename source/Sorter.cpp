////////////////////////////////////
// Sean Cooke
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 1 - MapReduce
// October 18, 2025
// 
#include "Sorter.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

bool Sorter::SortAndAggregate(const std::string& temp_dir, const std::string& aggregated_file) {
    std::vector<std::string> lines;

    // Read all .txt files in temp_dir
    for (const auto& entry : std::filesystem::directory_iterator(temp_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::ifstream in(entry.path());
            std::string line;
            while (std::getline(in, line)) {
                lines.push_back(line);
            }
        }
    }

    // Sort lines
    std::sort(lines.begin(), lines.end());

    // Write to aggregated_file
    std::ofstream out(aggregated_file);
    if (!out) return false;
    for (const auto& line : lines) {
        out << line << '\n';
    }
    return true;
}
