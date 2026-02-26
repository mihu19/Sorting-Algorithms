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
   .\HybridSort.exe in.txt               ::Reading file + Sorting
   .\HybridSort.exe in.txt stdout        ::Reading file + Sorting + Printing in Console
   .\HybridSort.exe in.txt output.txt    ::Reading file + Sorting + Writing output in file

## Compilation and Running (Mac)
**Compile Hybrid Sort:**
   
       clang++ -O3 -flto -DNDEBUG -std=c++17 -march=native \-Xpreprocessor -fopenmp \-I$(brew --prefix libomp)/include \-L$(brew --prefix libomp)/lib -lomp \sort.cpp -o sort && ./sort in.txt


**Compile Tools**  
*random.cpp*

    clang++ -O3 -march=native random.cpp -o random && ./random

*Ascending / Descending*
   ```bash
    clang++ -O3 -march=native gener.cpp -o gener && ./gener 

