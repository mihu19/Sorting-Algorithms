#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

int main() {
    const std::size_t N = 1000000;
    std::ofstream out("input.txt");
    if (!out) {
        std::cerr << "Error: can't open in.txt for writing\n";
        return 1;
    }

    std::mt19937 rng(35);
    std::uniform_int_distribution<int32_t> dist(-100000000, 100000000);

    out << N << '\n';
    for (std::size_t i = 0; i < N; ++i) out << dist(rng) << '\n';
    printf("The elemetns were added!");
    return 0;
}
