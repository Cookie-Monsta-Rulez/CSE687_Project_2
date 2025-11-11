# MapReduce Framework (C++)

## Overview

This project implements a modular **MapReduce framework** in C++ for **CSE 687 – Object-Oriented Design** at **Syracuse University**.  
It demonstrates distributed-style data processing using dynamically loaded **Map** and **Reduce** DLLs.

The framework mimics Google’s MapReduce model:

1. **Map Phase** – Reads input text and converts it into key-value pairs.  
2. **Reduce Phase** – Aggregates values for each key and writes summarized output.  
3. **Dynamic Linking** – Loads Map and Reduce functionality at runtime.

---

## Features

- Modular, object-oriented design  
- Dynamically loaded Map and Reduce DLLs  
- Example implementation: **Word Count MapReduce**  
- Supports large text input processing  
- Cross-platform design (Windows & Linux compatible)  
- Extensible interface for creating custom Map/Reduce logic  

---

## Directory Structure

CSE687_MapReduce/
├── include/
│ ├── FileManagement.h
│ ├── FileManagement2.h
│ ├── FMAdapter.h
│ ├── Reducer.h
│ └── Mapper.h
│
├── src/
│ ├── main.cpp
│ ├── Reducer.cpp
│ ├── MapFunc.cpp
│ ├── ReduceFunc.cpp
│ └── FileManagement.cpp
│
├── input/
│ └── pg100.txt
│
├── temp/
├── output/
├── MapFunc.dll
└── ReduceFunc.dll

yaml
Copy code

---

## Build Instructions

### Windows (MSVC)

```bash
cl /EHsc /std:c++17 src\main.cpp /Fe:MapReduce.exe
cl /EHsc /std:c++17 /LD src\ReduceFunc.cpp /Fe:ReduceFunc.dll
cl /EHsc /std:c++17 /LD src\MapFunc.cpp /Fe:MapFunc.dll
 Linux (g++)
bash
Copy code
g++ -std=c++17 src/main.cpp -o MapReduce -ldl
g++ -std=c++17 -shared -fPIC src/ReduceFunc.cpp -o ReduceFunc.so
g++ -std=c++17 -shared -fPIC src/MapFunc.cpp -o MapFunc.so
Execution
Run the program using:

bash
Copy code
MapReduce.exe <input_dir> <temp_dir> <output_dir> MapFunc.dll ReduceFunc.dll
Example:

bash
Copy code
MapReduce.exe input temp output MapFunc.dll ReduceFunc.dll
Program Flow
main.cpp dynamically loads the Map and Reduce DLLs.

Each input text file is processed line by line.

The Map function (MapFunc.dll) tokenizes each line into words and emits intermediate key-value pairs (word → 1) to
temp/intermediate.txt.

After mapping completes, the Reduce function (ReduceFunc.dll) aggregates identical keys and counts their occurrences.

The reduced output is written to output/final_results.txt.

A SUCCESS file is created in the output directory upon successful completion.

Example Output
File: output/final_results.txt

nginx
Copy code
word1   25
word2   14
word3   7
Technical Notes
FileManagement
Handles all directory and file operations (input, temp, output).

FMAdapter
Acts as a bridge between the DLL interface (FileManagement2) and the internal FileManagement implementation.

MapTokenize
Handles line tokenization, buffering, and writing intermediate key-value pairs to the temp file.

ReduceFunction
Reads the intermediate file, aggregates identical keys using std::unordered_map, and outputs the final word counts.

Dynamic Linking
Uses LoadLibraryA and GetProcAddress (Windows) or dlopen and dlsym (Linux) to load and invoke functions at runtime.

Authors
David Zapata

Lorenzo Mujica

