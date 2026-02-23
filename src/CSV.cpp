#include <iostream>
#include <fstream>
#include "utils.h"

using namespace std;

string getModeName(int mode) 
{
    switch (mode) 
    {
        case 1: return "In-Memory Benchmark";
        case 2: return "Read -> Sort -> Write";
        case 3: return "Sort -> Write";
        case 4: return "Read -> Sort";
        default: return "Unknown Mode";
    }
}

string getAlgorithmName(int choice) 
{
    switch (choice) 
    {
        case 1: return "Bubble Sort";
        case 2: return "Insertion Sort";
        case 3: return "Shell Sort";
        case 4: return "Merge Sort";
        case 5: return "Hybrid Sort"; 
        default: return "Unknown Algorithm";
    }
}

string getArrayTypeName(int type) 
{
    switch (type) 
    {
        case 1: return "Sorted";
        case 2: return "Reverse Sorted";
        case 3: return "Random";
        case 4: return "Half-Sorted";
        case 5: return "Mixed (+/-)";
        default: return "Unknown Type";
    }
}

void exportToCSV(int mode, int algorithmChoice, int arrayType, int size, double elapsedTime, double memoryUsedMB) 
{
    string csvFilename = "benchmark_results.csv";
    
    ifstream checkFile(csvFilename);
    bool writeHeader = !checkFile.good();
    checkFile.close();

    ofstream csvFile(csvFilename, ios::app);
    if (csvFile.is_open()) 
    {
        if (writeHeader) {
            csvFile << "Mode,Algorithm,Array Type,Size,Time (Seconds),Memory (MB)\n";
        }
        
        csvFile << getModeName(mode) << ","
                << getAlgorithmName(algorithmChoice) << ","
                << getArrayTypeName(arrayType) << ","
                << size << ","
                << elapsedTime << ","
                << memoryUsedMB << "\n";
                
        csvFile.close();
        cout << "Benchmark logged to " << csvFilename << "\n";
    } else 
        cout << "Unable to open CSV file for writing.\n";
}
