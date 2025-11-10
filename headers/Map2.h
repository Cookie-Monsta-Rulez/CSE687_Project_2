/////////////////////////////
// David Zapata
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce
// November 1, 2025
// Map2.h file - Header file to define the Map2 class definitions

#pragma once
#include <cstddef>
#include <string>

class FileManagement2; 

class Map2
{
public:

	virtual void map(const std::string& key, const std::string& value) = 0;
	virtual void finalize() = 0;
	
	virtual ~Map2() {}

}; // end class Map2

extern "C"
{
	using CreateMapFunc = Map2* (__stdcall*)(FileManagement2* fm, const char* interFile, std::size_t flushbytes);
	using DestroyMapFunc = void(__stdcall*)(Map2* instance);

} // end extern