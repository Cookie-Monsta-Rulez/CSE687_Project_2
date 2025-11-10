/////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce
// November 1, 2025
// MapTokenize.h file - Header file to define the MapTokenize class definitions
// 
// 
#pragma once
#include "Map2.h"
#include "FileManagement2.h"
#include <string>
#include <vector>

class MapTokenize : public Map2
{

public:
	MapTokenize(FileManagement2* fm, std::string interFile, std::size_t flushBytes);
	~MapTokenize() override;

	void map(const std::string& key, const std::string& line) override;
	void finalize() override;

private:
	static void tokenize(const std::string& line, std::vector<std::string>& out);
	void exportData(const std::string& key, const std::string& line);
	void flush();

private:
	FileManagement2* fm;
	std::string interName;
	std::size_t flushBytes;
	std::string buffer;
	bool opened{ false };

}; // end class MapTokenize
