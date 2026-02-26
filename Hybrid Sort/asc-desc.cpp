#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

int main() {
    const std::size_t N = 1000000;
    std::ofstream out("input.txt");
    if (!out) {
        std::cerr << "Error: can't open in.txt for writing\n";
        return 1;
    }

    // 1. Generate random numbers first
    std::vector<int32_t> data(N);
    std::mt19937 rng(33);
    std::uniform_int_distribution<int32_t> dist(-100000000, 100000000);

    for (std::size_t i = 0; i < N; ++i) {
        data[i] = dist(rng);
    }

    // ASCENDING
    // 2. Sort them to make the data ascending
    std::sort(data.begin(), data.end());

    // 3. Write to file
    
    out << N << '\n';
    for (std::size_t i = 0; i < N; ++i) {
        out << data[i] << '\n';
    }

    std::cout << "1,000,000 ascending elements were added to in.txt!" << std::endl;

    
    // DESCENDING
    // 2. Sort them to make the data DESCENDING
    // std::sort(data.begin(), data.end(), std::greater<int32_t>());

    // // 3. Write to file
    // out << N << '\n';
    // for (std::size_t i = 0; i < N; ++i) {
    // out << data[i] << '\n';
    // }

    // std::cout << "1,000,000 descending elements were added to in.txt!" << std::endl;

    return 0;
}
