## Prerequisites

To compile and run this code:
* **C++ Compiler:** 
* **Windows PowerShell** or standard command prompt.
* **Input.txt** file with elements to be sorted (in the same directory)
* **(macOS) OpenMP library** - required for parallel execution.
    ```bash
    brew install libomp


## Compilation and Running (Windows)

1. **Open the Project:** Launch Visual Studio Code and open the extracted `Sorting-Algorithms` folder.
2. **Open the Terminal:** In the top menu, click **Terminal** > **New Terminal** (or press `` Ctrl + ` ``).
3. **Navigate to the Source Folder:**
   ```bash
   cd "Hybrid Sort"
4. **Compile:**
   ```bash
    g++ -O3 -march=native -fopenmp  -DNDEBUG -fno-exceptions -fno-rtti -lpsapi sort.cpp -o HybridSort
5. **Run:**
   ```bash
   .\HybridSort.exe input.txt               ::Reading file + Sorting
   .\HybridSort.exe input.txt stdout        ::Reading file + Sorting + Printing in Console
   .\HybridSort.exe input.txt output.txt    ::Reading file + Sorting + Writing output in file

## Compilation and Running (macOS)
**Compile Hybrid Sort:**
   
       clang++ -O3 -flto -DNDEBUG -std=c++17 -march=native \-Xpreprocessor -fopenmp \-I$(brew --prefix libomp)/include \-L$(brew --prefix libomp)/lib -lomp \sort.cpp -o sort && ./sort input.txt

**Tools**  
*random.cpp* - Generates 1,000,000 random int32 values and writes to `input.txt`.

    clang++ -O3 -march=native random.cpp -o random && ./random

*Ascending / Descending* - Generates 1,000,000 ascending (or descending) int32 values and writes to `input.txt`.
   ```bash
    clang++ -O3 -march=native gener.cpp -o gener && ./gener 


