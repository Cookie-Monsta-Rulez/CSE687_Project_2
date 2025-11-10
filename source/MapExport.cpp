// Phase 2 - MapReduce
// November 1, 2025
// MapExport.cpp file - MapExport base class source-code file to initialize and define member functions.

#include "Map2.h"
#include "FileManagement2.h"
#include "MapTokenize.h"
#include "FMAdapter.h"
#include <string>
#include <vector>

#ifdef _WIN32
    #define EXPORT __attribute__((dllexport))
    #define STDCALL __attribute__((stdcall))
#else
    #define EXPORT __attribute__((visibility("default")))
    #define STDCALL
#endif

extern "C" {

EXPORT Map2* STDCALL CreateMap(FileManagement2* fm, const char* interFile, std::size_t flushBytes)
{
    return new MapTokenize(fm, std::string(interFile), flushBytes);
}

EXPORT void STDCALL DestroyMap(Map2* instance)
{
    delete instance;
}

} // extern "C"
