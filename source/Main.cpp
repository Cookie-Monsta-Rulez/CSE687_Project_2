////////////////////////////////////
// David Zapata, Lorenzo Mujica, Sean Cooke
// Professor Scott Roueche
// CSE 687 - Object Oriented Design
// Syracuse University
// Phase 2 - MapReduce with Dynamic Link Libraries
// November 10, 2025
// 
// Main.cpp file - Main source-code file that orchestrates
// the MapReduce workflow by dynamically loading Map and Reduce DLLs,
// managing intermediate file operations, and executing the full pipeline.
////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>

#include "FileManagement.h"
#include "FMAdapter.h"
#include "FileManagement2.h"
#include "Map2.h"

namespace fs = std::filesystem;

// -------------------------------------------------------------
// Typedefs for dynamically loaded functions from Map/Reduce DLLs
// -------------------------------------------------------------
typedef Map2* (*CreateMapFunc)(FileManagement2*, const char*, size_t);
typedef void  (*DestroyMapFunc)(Map2*);
typedef void  (*ReduceFunc)(const std::string&, const std::vector<int>&);

// =============================================================
// Class: Workflow
// Purpose: Encapsulates the logic for executing the Map and Reduce
//          phases of the MapReduce workflow.
// =============================================================
class Workflow {
public:
    ////////////////////////////////////
    // Constructor
    // Parameters:
    //  - fmAdapter: Pointer to a FileManagement2 adapter instance
    //  - createMap: Function pointer to dynamically load a Map object
    //  - destroyMap: Function pointer to release the Map object
    //  - reduceFunc: Function pointer to Reduce function in DLL
    //  - tempDir: Temporary directory for intermediate files
    ////////////////////////////////////
    Workflow(FileManagement2* fmAdapter,
             CreateMapFunc createMap,
             DestroyMapFunc destroyMap,
             ReduceFunc reduceFunc,
             const std::string& tempDir)
        : fmAdapter_(fmAdapter),
          createMap_(createMap),
          destroyMap_(destroyMap),
          reduceFunc_(reduceFunc),
          tempDir_(tempDir) {}

    ////////////////////////////////////
    // Function: Execute
    // Purpose: Orchestrates the MapReduce workflow by:
    //   1. Scanning the input directory for files.
    //   2. Running the Map phase for each file.
    //   3. Finalizing intermediate data output.
    //   4. Executing the Reduce phase.
    //
    // Parameters:
    //  - inputDir: Directory containing input text files
    //  - outputDir: Directory to store final reduced output
    ////////////////////////////////////
    void Execute(const std::string& inputDir, const std::string& outputDir) {
        std::cout << "[*] Starting MapReduce workflow...\n";

        // Collect all input files in the input directory
        std::vector<fs::path> files;
        for (auto& p : fs::directory_iterator(inputDir)) {
            if (p.is_regular_file())
                files.push_back(p.path());
        }

        if (files.empty()) {
            std::cerr << "[!] No input files found in: " << inputDir << "\n";
            return;
        }

        // Define intermediate file path
        std::string interFile = (fs::path(tempDir_) / "intermediate.txt").string();

        // Create Map instance via DLL
        Map2* mapInstance = createMap_(fmAdapter_, interFile.c_str(), 1024);
        if (!mapInstance) {
            std::cerr << "[!] Failed to create Map instance\n";
            return;
        }

        // ----------------------
        // MAP PHASE
        // ----------------------
        for (const auto& file : files) {
            std::ifstream in(file);
            if (!in) {
                std::cerr << "[!] Failed to open input file: " << file.string() << "\n";
                continue;
            }

            std::string line;
            while (std::getline(in, line)) {
                // Process each line using the map function
                mapInstance->map(file.string(), line);
            }
        }

        // Finalize writing any buffered data to intermediate storage
        mapInstance->finalize();

        // ----------------------
        // REDUCE PHASE
        // ----------------------
        if (reduceFunc_) {
            reduceFunc_(interFile, std::vector<int>{});
        }

        // Clean up the Map instance
        destroyMap_(mapInstance);
    }

private:
    FileManagement2* fmAdapter_;     // Interface to manage intermediate files
    CreateMapFunc createMap_;        // Function to create Map instance
    DestroyMapFunc destroyMap_;      // Function to destroy Map instance
    ReduceFunc reduceFunc_;          // Pointer to Reduce function
    std::string tempDir_;            // Temporary directory for intermediates
};

// =============================================================
// Class: Executive
// Purpose: Manages program startup, argument parsing, DLL loading,
//          and invokes the Workflow execution.
// =============================================================
class Executive {
public:
    ////////////////////////////////////
    // Function: Run
    // Purpose: Entry point of the program that:
    //   - Validates command-line arguments.
    //   - Dynamically loads Map and Reduce DLLs.
    //   - Initializes file management.
    //   - Executes the full MapReduce workflow.
    //
    // Returns:
    //   - 0 on success
    //   - 1 on failure
    ////////////////////////////////////
    int Run(int argc, char* argv[]) {
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0]
                      << " <input_dir> <temp_dir> <output_dir> <map_dll> <reduce_dll>\n";
            return 1;
        }

        std::string inputDir  = argv[1];
        std::string tempDir   = argv[2];
        std::string outputDir = argv[3];
        std::string mapDLL    = argv[4];
        std::string reduceDLL = argv[5];

        // ----------------------
        // Load Map DLL
        // ----------------------
        HMODULE mapModule = LoadLibraryA(mapDLL.c_str());
        if (!mapModule) {
            std::cerr << "[!] Failed to load Map DLL: " << mapDLL << "\n";
            return 1;
        }

        auto createMap = (CreateMapFunc)GetProcAddress(mapModule, "CreateMap");
        auto destroyMap = (DestroyMapFunc)GetProcAddress(mapModule, "DestroyMap");

        if (!createMap || !destroyMap) {
            std::cerr << "[!] Failed to get Map DLL functions\n";
            FreeLibrary(mapModule);
            return 1;
        }

        // ----------------------
        // Load Reduce DLL
        // ----------------------
        HMODULE reduceModule = LoadLibraryA(reduceDLL.c_str());
        if (!reduceModule) {
            std::cerr << "[!] Failed to load Reduce DLL: " << reduceDLL << "\n";
            FreeLibrary(mapModule);
            return 1;
        }

        ReduceFunc reduceFunc = (ReduceFunc)GetProcAddress(reduceModule, "ReduceFunction");
        if (!reduceFunc) {
            std::cerr << "[!] Could not find ReduceFunction in " << reduceDLL << "\n";
            FreeLibrary(mapModule);
            FreeLibrary(reduceModule);
            return 1;
        }

        // ----------------------
        // Validate and prepare directories
        // ----------------------
        if (!fs::exists(tempDir)) fs::create_directories(tempDir);
        if (!fs::exists(outputDir)) fs::create_directories(outputDir);

        // ----------------------
        // Initialize File Management
        // ----------------------
        FileManagement fm(inputDir, outputDir, tempDir);
        FMAdapter fmAdapter(fm);

        // ----------------------
        // Execute Workflow
        // ----------------------
        Workflow workflow(&fmAdapter, createMap, destroyMap, reduceFunc, tempDir);
        workflow.Execute(inputDir, outputDir);

        // ----------------------
        // Clean up loaded DLLs
        // ----------------------
        FreeLibrary(mapModule);
        FreeLibrary(reduceModule);

        std::cout << "[*] MapReduce completed successfully.\n";
        return 0;
    }
};

// =============================================================
// Function: main
// Purpose: Program entry point. Delegates to Executive::Run()
// =============================================================
int main(int argc, char* argv[]) {
    Executive exec;
    return exec.Run(argc, argv);
}
