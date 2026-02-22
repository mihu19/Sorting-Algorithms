#include "utils.h"
using namespace std;

void shellSort(vector<int>& arr) 
{
    int n = arr.size();

    // Start with a large gap then reduce it
    for (int gap = n / 2; gap > 0; gap /= 2) 
    {
        for (int i = gap; i < n; i++) 
        {
            int temp = arr[i];  
            int j = i;
            while (j >= gap && arr[j - gap] > temp) 
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}
