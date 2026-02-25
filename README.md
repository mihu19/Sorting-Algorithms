# Sorting-Algorithms

A  C++ and Python project designed to implement, test, and benchmark various sorting algorithms. 

This tool generates data arrays, sorts them using multiple techniques, and automatically logs performance metrics—such as execution time and memory usage into CSV files. The included Python tools can then be used to analyze and visualize the benchmark results.


## Implemented Algorithms

The repository includes the following sorting algorithms:
* Bubble Sort
* Insertion Sort
* Shell Sort
* Merge Sort
* Hybrid Sort (Radix Sort + Insertion Sort)

## Project Structure

* **`src/`**: Contains all C++ source code, including algorithm implementations, array generation, memory management, and CSV handling.
* **`Tools/`**: Python scripts are used for analyzing, plotting, and visualizing the CSV benchmark data.
* **`Results/`**: The output directory where the generated CSV benchmark files are saved.
* **`Hybrid Sort/`**: Contains specific logic, data, or tests dedicated to the hybrid sorting approach.

## Prerequisites

To compile and run this project:
* **C++ Compiler:** 
* **Windows PowerShell** or standard command prompt.
* **Python 3.x** (Optional: only needed if you want to run the analysis scripts in the `Tools/` folder).


### Installation & Compilation (Windows)
### Option A: Using Visual Studio Code (No Git Required)

1. **Download the Code:** Click the green **Code** button at the top of this repository and select **Download ZIP**. Extract the folder to your computer.
2. **Open the Project:** Launch Visual Studio Code and open the extracted `Sorting-Algorithms` folder.
3. **Open the Terminal:** In the top menu, click **Terminal** > **New Terminal** (or press `` Ctrl + ` ``).
4. **Navigate to the Source Folder:**

### Option B: Using CMD (Git Required)
1. Clone the repository:
   ```bash
   git clone https://github.com/mihu19/Sorting-Algorithms.git
2. Navigate to the `src` directory:
   ```bash
   cd Sorting-Algorithms/src
3. Compile:
   ```bash
   g++ main.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp MergeSort.cpp HybridSort.cpp ArrayGenerator.cpp Files.cpp Memory.cpp CSV.cpp -o sort -lpsapi -fopenmp -O3
4. Run:
   ```bash
   .\sort.exe
