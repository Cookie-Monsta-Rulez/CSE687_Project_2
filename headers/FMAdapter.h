/////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce
// November 1, 2025
// FMAdapter.h file - File that connects FileManagement2 DLL interface 
// with FileManagement implementation

#pragma once
#include "FileManagement2.h"
#include "FileManagement.h"
#include <fstream>
#pragma once  // simplest

class FMAdapter : public FileManagement2
{
public:
	explicit FMAdapter(FileManagement& fm) : fm(fm) {}

	bool openInter(const std::string& file) override
	{
		// Ensure we only pass the filename, not a full or relative path
		std::filesystem::path p(file);
		out = fm.openTemp(p.filename().string());
		return true;
	}


	bool appendInter(const char* data, std::size_t len) override
	{
		if (!out.is_open())
			return false;
		out.write(data, static_cast<std::streamsize>(len));
		return static_cast<bool>(out);
	} // end appendInter

	void closeInter() override
	{
		if (out.is_open())
			out.close();
	} // end closeInter

private:
	FileManagement& fm;
	std::ofstream out;

}; // end FMAdapter class
