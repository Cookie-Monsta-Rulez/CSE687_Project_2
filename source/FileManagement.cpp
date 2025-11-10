////////////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 1 - MapReduce
// October 14, 2025
// FileManagement.cpp file - FileManagement base class source-code file to initialize and define member functions.

#include "FileManagement.h"
#include <stdexcept>
#include <algorithm>
#include <system_error>
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
namespace fs = std::filesystem;

// constructor initializes input, output, and temp data members 
FileManagement::FileManagement(string input, string output, string temp)
	: inputDir(fs::absolute(input)), outputDir(fs::absolute(output)), tempDir(fs::absolute(temp)) 
{
// empty body

} // end FileManagement constructor

// function to check in input folder exists, creates output and temp directories if they do not already exist
void FileManagement::initialize()
{
	if (!fs::exists(inputDir) || !fs::is_directory(inputDir))
	{
		throw std::runtime_error("Input directory does not exist: " + inputDir.string());
	} // end if

	std::error_code errorCode;
	fs::create_directories(outputDir, errorCode);
	if (errorCode)
	{
		throw std::runtime_error("Could not create output directory: " + outputDir.string());
	} // end if

	errorCode.clear();
	fs::create_directories(tempDir, errorCode);
	if (errorCode)
	{
		throw std::runtime_error("Could not create temp directory: " + tempDir.string());
	} // end if
} // end of initialize function

// function to collect input files and adds their paths to a vector
vector<fs::path> FileManagement::inputFiles() const
{
	vector<fs::path> files;
	for (const auto& entry : fs::directory_iterator(inputDir))
	{
		if (entry.is_regular_file())
		{
			files.push_back(entry.path());
		} // end  if
	} // end for loop

	return files;
} // end of inputFiles function

// function to open a file to read
ifstream FileManagement::openFile(const fs::path& filepath) const
{
	ifstream in(filepath, std::ios::in);
	if (!in.is_open())
	{
		throw std::runtime_error("Failed to open input file: " + filepath.string());
	} // end if
	return in;
} // end of openFile function

// function to create/open a tempfile
ofstream FileManagement::openTemp(const string& fileName) const
{
	fs::path path = tempDir / fileName;
	ofstream out(path, std::ios::out | std::ios::app);
	if (!out.is_open())
	{
		throw std::runtime_error("Failed to open temp file: " + path.string());
	} // end if
	return out;
} // end of openTemp function

// function to create a file in ouput director
void FileManagement::writeFile() const
{
	fs::path success = outputDir / "SUCCESS";
	ofstream out(success, std::ios::out | std::ios::trunc);
	if (!out.is_open())
	{
		throw std::runtime_error("Failed to write file at: " + success.string());
	} // end if
} // end of writeFile Function