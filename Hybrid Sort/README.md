## Prerequisites

To compile and run this code:
* **C++ Compiler:** 
* **Windows PowerShell** or standard command prompt.
* **Input.txt** file with elements to be sorted


## Installation & Compilation (Windows)
### Option A: Using Visual Studio Code (No Git Required)

1. **Download the Code:** Click the green **Code** button at the top of this repository and select **Download ZIP**. Extract the folder to your computer.
2. **Open the Project:** Launch Visual Studio Code and open the extracted `Sorting-Algorithms` folder.
3. **Open the Terminal:** In the top menu, click **Terminal** > **New Terminal** (or press `` Ctrl + ` ``).
4. **Navigate to the Source Folder:**
   ```bash
   cd src
5. Compile:
   ```bash
    g++ main.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp MergeSort.cpp HybridSort.cpp ArrayGenerator.cpp Files.cpp Memory.cpp CSV.cpp -o sort -lpsapi -fopenmp -O3
6. Run:
   ```bash
   .\sort.exe

