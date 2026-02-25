#pragma once
#include <vector>
#include <string>

// Sorting Algorithms
void bubbleSort(std::vector<int>& arr);
void insertionSort(std::vector<int>& arr);
void shellSort(std::vector<int>& arr);
void mergeSort(std::vector<int>& arr, int left, int right);
void hybridSort(std::vector<int>& data);

// Array Generators
std::vector<int> generateSortedVector(int size);
std::vector<int> generateReverseSortedVector(int size);
std::vector<int> generateRandomVector(int size);
std::vector<int> generateHalfSortedVector(int size);
std::vector<int> generateMixedVector(int size);

// Memory
void printMemoryUsage();

// File Management
std::vector<int> readFromFile(const std::string& filename);
void writeToFile(const std::string& filename, const std::vector<int>& data);

//Export to CSV
double getMemoryUsageMB();
std::string getModeName(int mode);
std::string getAlgorithmName(int choice);
std::string getArrayTypeName(int type);
void exportToCSV(int mode, int algorithmChoice, int arrayType, int size, double elapsedTime, double memoryUsedMB);
