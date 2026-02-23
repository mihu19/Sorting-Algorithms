#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include "utils.h"

using namespace std;
using namespace std::chrono;

// SORTING ALGORITHMS
void executeSort(int choice, vector<int>& data) 
{
    switch (choice) 
    {
        case 1: bubbleSort(data); break;
        case 2: insertionSort(data); break;
        case 3: shellSort(data); break;
        case 4: 
            if (!data.empty())
                mergeSort(data, 0, data.size() - 1); 
            break;
        case 5: 
            hybridSort(data); 
            break;
        default:
            cout << "Invalid algorithm choice.\n";
            exit(1);
    }
}

// ARRAY GENERATORS
vector<int> generateArrayByType(int type, int size) 
{
    switch (type) 
    {
        case 1: return generateSortedVector(size);
        case 2: return generateReverseSortedVector(size);
        case 3: return generateRandomVector(size);
        case 4: return generateHalfSortedVector(size);
        case 5: return generateMixedVector(size);
        default:
            cout << "Invalid array type. Using random.\n";
            return generateRandomVector(size);
    }
}

// MAIN
int main() 
{
    int mode, algorithmChoice, arrayType = 0, size = 0;
    string customFileName = "";
    
    cout << "\n========== Sorting Algorithms Benchmark ==========\n\n";

    cout << "Select Mode:\n";
    cout << "1. In-Memory Benchmark\n";
    cout << "2. Generate File -> Read -> Sort -> Write\n";
    cout << "3. Generate Array -> Sort -> Write to File\n";
    cout << "4. Generate File -> Read -> Sort -> Print to Console\n";
    cout << "5. Read from custom file -> Sort\n";
    cout << "Enter choice: ";
    cin >> mode;

    if (mode == 5) 
    {
        cout << "Enter the input file name (data.txt): ";
        cin >> customFileName;
    }

    cout << "\nSelect Sorting Algorithm:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Insertion Sort\n";
    cout << "3. Shell Sort\n";
    cout << "4. Merge Sort\n";
    cout << "5. Hybrid Sort\n"; 
    cout << "Enter choice (1-5): ";     
    cin >> algorithmChoice;

    // Skip size and array type generation prompts if reading from an existing custom file
    if (mode != 5) {
        cout << "\nEnter size: ";
        cin >> size;

        cout << "\nSelect Array Type:\n";
        cout << "1. Sorted\n";
        cout << "2. Reverse Sorted\n";
        cout << "3. Random\n";
        cout << "4. Half-Sorted\n";
        cout << "5. Mixed (+/-)\n";
        cout << "Enter choice (1-5): ";
        cin >> arrayType;
    }

    vector<int> data;
    duration<double> elapsed;

    // Mode 1: In-Memory Benchmark
    if (mode == 1) 
    {
        data = generateArrayByType(arrayType, size);

        // Timer - only sorting
        auto start = high_resolution_clock::now();
        executeSort(algorithmChoice, data);
        auto stop = high_resolution_clock::now();
        
        elapsed = stop - start;
        cout << "\n=== In-Memory Benchmark ===\n";
    }

    // Mode 2: Read -> Sort -> Write
    else if (mode == 2) 
    {
        string inputFile = "input.txt";
        string outputFile = "output.txt";

        vector<int> initialData = generateArrayByType(arrayType, size);
        writeToFile(inputFile, initialData);

        // Timer - reading + sorting + writing
        auto start = high_resolution_clock::now();
        
        data = readFromFile(inputFile);
        executeSort(algorithmChoice, data);
        writeToFile(outputFile, data);
        
        auto stop = high_resolution_clock::now();
        
        elapsed = stop - start;
        cout << "\n=== Full Benchmark (Read + Sort + Write) ===\n";
    }

    // Mode 3: Sort -> Write
    else if (mode == 3) 
    {
        string outputFile = "sorted.txt";
        data = generateArrayByType(arrayType, size);

        // Timer - sorting + writing
        auto start = high_resolution_clock::now();
        
        executeSort(algorithmChoice, data);
        writeToFile(outputFile, data);
        
        auto stop = high_resolution_clock::now();
        
        elapsed = stop - start;
        cout << "\n=== Sort + Write Benchmark ===\n";
    }

    // Mode 4: Read -> Sort (Console print not timed)
    else if (mode == 4) 
    {
        string inputFile = "input.txt";
        
        vector<int> initialData = generateArrayByType(arrayType, size);
        writeToFile(inputFile, initialData);

        // Timer - reading + sorting
        auto start = high_resolution_clock::now();
        
        data = readFromFile(inputFile);
        executeSort(algorithmChoice, data);

        auto stop = high_resolution_clock::now();
        
        elapsed = stop - start;

        cout << "\nSorted Output:\n";
        for (int x : data) {
            cout << x << " ";
        }
        cout << "\n\n=== Read + Sort Benchmark ===\n";
    }

    // Mode 5: Read from Custom File -> Sort
   else if (mode == 5)
    {
        data = readFromFile(customFileName);
        size = data.size(); 
        
        //Timer - only sorting
        auto start = high_resolution_clock::now();
        
        executeSort(algorithmChoice, data);
        
        auto stop = high_resolution_clock::now();
        
        elapsed = stop - start;
        cout << "\n=== Custom File Benchmark (Sort Only) ===\n";
    }

    // Metrics/Export
    double memoryUsed = getMemoryUsageMB();

    // Console output for metrics
    cout << "Algorithm Execution Time: " << elapsed.count() << " seconds\n";
    cout << "Memory Usage: " << memoryUsed << " MB\n";
    cout << "\n";

    // Export metrics to CSV
    exportToCSV(mode, algorithmChoice, arrayType, size, elapsed.count(), memoryUsed);

    return 0;
}
