/////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce
// November 1, 2025
// MapTokenize.cpp file - MapTokenzie base class source-code file to initialize and define member functions.

#include "MapTokenize.h"
#include <algorithm>
#include <cctype>
#include <iostream>


using std::string;
using std::vector;

MapTokenize::MapTokenize(FileManagement2* fm, string interFile, std::size_t flushBytes)
    : fm(fm), interName(std::move(interFile)), flushBytes(flushBytes)
{
    buffer.reserve(flushBytes + 1024);
    if (fm)
    {
        opened = fm->openInter(interName);
        std::cerr << "[MapTokenize] openInter(" << interName << ") returned " << opened << "\n";
    }
    else
    {
        std::cerr << "[MapTokenize] fm pointer is null!\n";
    }
}

MapTokenize::~MapTokenize()
{
	if (!buffer.empty())
	{
		flush();
	} // end if
}

// function to tokenize input line into words
void MapTokenize::map(const string& key, const string& line)
{
	vector<string> tokens;
	tokens.reserve(32);
	tokenize(line, tokens);

	for (const auto& t : tokens)
	{
		exportData(t, "1");
	} // end for loop

} // end map function

// function that removes punctuation, whitespace, and capitalization
void MapTokenize::tokenize(const string& line, vector<string>& out)
{
	out.clear();
	string current;
	current.reserve(32);

	auto push = [&]()
		{
			if (!current.empty())
			{
				std::transform(current.begin(), current.end(), current.begin(),
					[](unsigned char c) {return static_cast<char>(std::tolower(c)); });
				out.emplace_back(std::move(current));
				current.clear();
			} // end if
		}; // end push

	for (unsigned char ch : line)
	{
		if (std::isalnum(ch))
		{
			current.push_back(static_cast<char>(ch));
		} // end if
		else
		{
			push();
		} // end else
	} // end for

	push();

} // end tokenize function

// function that exports key and value
void MapTokenize::exportData(const string& key, const string& line)
{
	if (key.empty())
		return;

	buffer.append(key);
	buffer.push_back('\t');
	buffer.append(line);
	buffer.push_back('\n');

	if (buffer.size() >= flushBytes)
	{
		flush();
	} // end if

} // end exportData function

// function to clear data from memory
void MapTokenize::flush()
{
    std::cerr << "[flush] opened=" << opened 
              << " buffer_size=" << buffer.size() 
              << " interFile=" << interName << "\n";
    if (!opened || buffer.empty())
        return;
    fm->appendInter(buffer);
    buffer.clear();
}

// function that takes data in memory gets written
void MapTokenize::finalize()
{
	if (!opened)
		return;

	if (!buffer.empty())
	{
		std::cerr << "[finalize] Flushing buffer of size " << buffer.size() << "\n";
		flush(); 
	}

	fm->closeInter();
	opened = false;
	
} //  end finalize function
