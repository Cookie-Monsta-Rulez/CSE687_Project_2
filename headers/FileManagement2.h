/////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce
// November 1, 2025
// FileManagement2.h file - Header file to define the FileManager class definitions

#pragma once
#include <cstddef>
#include <string>

class FileManagement2
{
public:
	virtual ~FileManagement2() {}

	virtual bool openInter(const std::string& file) = 0;

	virtual bool appendInter(const char* data, std::size_t len) = 0;
	
	bool appendInter(const std::string& s)
	{
		return appendInter(s.data(), s.size());
	}

	virtual void closeInter() = 0;

}; // end class FileManagement2