#include <random>
#include <algorithm>
#include "utils.h"
using namespace std;

// Sorted
vector<int> generateSortedVector(int size) 
{
    vector<int> arr(size);
    for (int i = 0; i < size; ++i)
        arr[i] = i;
    return arr;
}

// Reverse Sorted
vector<int> generateReverseSortedVector(int size) 
{
    vector<int> arr(size);
    for (int i = 0; i < size; ++i)
        arr[i] = size - i;
    return arr;
}

// Random - fixed to positive values
vector<int> generateRandomVector(int size) 
{
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 100000); 

    for (int i = 0; i < size; ++i)
        arr[i] = distrib(gen);

    return arr;
}

// Half-Sorted
vector<int> generateHalfSortedVector(int size) 
{
    vector<int> arr(size);
    int half = size / 2;

    for (int i = 0; i < half; ++i)
        arr[i] = i;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-100000, 100000);

    for (int i = half; i < size; ++i)
        arr[i] = distrib(gen);

    return arr;
}

// Mixed Positive + Negative
vector<int> generateMixedVector(int size) 
{
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-100000, 100000);

    for (int i = 0; i < size; ++i)
        arr[i] = distrib(gen);

    return arr;
}
