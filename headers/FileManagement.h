////////////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 1 - MapReduce
// October 14, 2025
// FileManagement.h file - Header file to define the FileManagement class definitions

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class FileManagement
{
public:
	FileManagement(std::string input, std::string output, std::string temp);

	void initialize();
	std::vector<std::filesystem::path> inputFiles() const;
	std::ifstream openFile(const std::filesystem::path& filepath) const;
	std::ofstream openTemp(const std::string& fileName) const;
	void writeFile() const;

private:
	std::filesystem::path inputDir;
	std::filesystem::path outputDir;
	std::filesystem::path tempDir;
}; // end class