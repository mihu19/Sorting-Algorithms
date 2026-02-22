#include <iostream>
#include "utils.h"

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

double getMemoryUsageMB() {
    double peakMB = 0.0;

    #if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) 
        peakMB = pmc.PeakWorkingSetSize / (1024.0 * 1024.0);

    //for linux and mac
    #elif defined(__linux__) || defined(__APPLE__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) 
    {
        #if defined(__APPLE__)
        peakMB = usage.ru_maxrss / (1024.0 * 1024.0); 
        #else
        peakMB = usage.ru_maxrss / 1024.0;          
        #endif
    }
    #endif
    
    // Return the value so main.cpp can use it for the CSV
    return peakMB;
}