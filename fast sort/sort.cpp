#include <iostream>
#include <vector>
#include <chrono>
#include <type_traits>
#include <omp.h>
#include <memory>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <algorithm> // Required for std::reverse and std::swap

#ifdef _WIN32
#include <malloc.h>
#define ALLOC(s) _aligned_malloc((s), 64)
#define FREE(p) _aligned_free(p)
#else
#include <stdlib.h>
inline void* ALLOC(size_t s) { void* p; return posix_memalign(&p, 64, s) == 0 ? p : nullptr; }
#define FREE(p) free(p)
#endif
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif
using namespace std;
using namespace std::chrono;

// --- DATA PROFILING ---
enum class DataState { ASCENDING, DESCENDING, RANDOM };

DataState checkDataState(const vector<int32_t>& arr) {
    size_t n = arr.size();
    if (n < 2) return DataState::ASCENDING;

    bool is_asc = true, is_desc = true;
    for (size_t i = 1; i < n; ++i) {
        if (arr[i] < arr[i - 1]) is_asc = false;
        if (arr[i] > arr[i - 1]) is_desc = false;
        // Early exit: if it's neither, it's random.
        if (!is_asc && !is_desc) return DataState::RANDOM;
    }
    return is_asc ? DataState::ASCENDING : DataState::DESCENDING;
}

// --- ALIGNED MEMORY MANAGEMENT ---
template<typename T> struct AlignedDel { void operator()(T* p) const { FREE(p); } };
template<typename T> using AlignedPtr = unique_ptr<T[], AlignedDel<T>>;
template<typename T> AlignedPtr<T> alloc_aligned(size_t n) { return AlignedPtr<T>(static_cast<T*>(ALLOC(n * sizeof(T)))); }

// --- UNIVERSAL RADIX SORT ---
template <int BITS, bool PARALLEL, typename T>
void radixSort(vector<T>& arr) {
    constexpr int SIZE = 1 << BITS, MASK = SIZE - 1, TOTAL_BITS = sizeof(T) * 8;
    size_t n = arr.size();
    using UT = typename make_unsigned<T>::type;

    AlignedPtr<T> temp = alloc_aligned<T>(n);
    int num_threads = PARALLEL ? omp_get_max_threads() : 1;
    size_t meta_size = num_threads * SIZE;

    AlignedPtr<size_t> counts = alloc_aligned<size_t>(meta_size);
    AlignedPtr<size_t> offsets = alloc_aligned<size_t>(meta_size);

    if (!temp || !counts || !offsets) {
        cerr << "Error: out of memory.\n";
        exit(1);
    }

    T *src = arr.data(), *dst = temp.get();
    size_t *c_ptr = counts.get(), *o_ptr = offsets.get();

    for (int shift = 0; shift < TOTAL_BITS; shift += BITS) {
        memset(c_ptr, 0, meta_size * sizeof(size_t));

        #pragma omp parallel if(PARALLEL)
        {
            int tid = PARALLEL ? omp_get_thread_num() : 0;
            int num_threads_active = PARALLEL ? omp_get_num_threads() : 1;
            size_t* l_counts = &c_ptr[tid * SIZE];

            size_t chunk_size = n / num_threads_active;
            size_t start = tid * chunk_size;
            size_t end = (tid == num_threads_active - 1) ? n : start + chunk_size;

            for (size_t i = start; i < end; ++i) {
                l_counts[(static_cast<UT>(src[i]) >> shift) & MASK]++;
            }
        }

        size_t global_off = 0;
        int xor_mask = (is_signed<T>::value && shift + BITS >= TOTAL_BITS) ? (1 << (TOTAL_BITS - 1 - shift)) : 0;
        for (int i = 0; i < SIZE; i++) {
            int bucket = i ^ xor_mask;
            for (int t = 0; t < num_threads; t++) {
                o_ptr[t * SIZE + bucket] = global_off;
                global_off += c_ptr[t * SIZE + bucket];
            }
        }

        #pragma omp parallel if(PARALLEL)
        {
            int tid = PARALLEL ? omp_get_thread_num() : 0;
            int num_threads_active = PARALLEL ? omp_get_num_threads() : 1;
            size_t l_offset[SIZE];
            memcpy(l_offset, &o_ptr[tid * SIZE], SIZE * sizeof(size_t));

            size_t chunk_size = n / num_threads_active;
            size_t start = tid * chunk_size;
            size_t end = (tid == num_threads_active - 1) ? n : start + chunk_size;

            for (size_t i = start; i < end; ++i) {
                UT val = static_cast<UT>(src[i]);
                dst[l_offset[(val >> shift) & MASK]++] = src[i];
            }
        }
        swap(src, dst);
    }
    if (src != arr.data()) memcpy(arr.data(), src, n * sizeof(T));
}

// --- INSERTION SORT ---
template <typename T>
void insertionSort(vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) { arr[j] = arr[j - 1]; --j; }
        arr[j] = key;
    }
}

// --- MAIN DISPATCHER ---
void printPeakMemory() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        std::cerr << "Peak RAM usage: " << fixed << setprecision(2)
                  << pmc.PeakWorkingSetSize / (1024.0 * 1024.0) << " MB\n";
    }
#else
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    // На macOS getrusage возвращает байты, на Linux - килобайты.
#if defined(__APPLE__)
    std::cerr << "Peak RAM usage: " << fixed << setprecision(2)
              << r_usage.ru_maxrss / (1024.0 * 1024.0) << " MB\n";
#else
    std::cerr << "Peak RAM usage: " << fixed << setprecision(2)
              << r_usage.ru_maxrss / 1024.0 << " MB\n";
#endif
#endif
}
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file> [stdout|output_file]\n";
        return 2;
    }

    ifstream in(argv[1]);
    size_t N;
    if (!in || !(in >> N)) return 1;

    vector<int32_t> data(N);
    for (size_t i = 0; i < N; ++i) {
        long long x; in >> x;
        data[i] = (int32_t)x;
    }

    string method;
    auto t_start = high_resolution_clock::now();

    // 1. Detect data state
    DataState state = checkDataState(data);

    // 2. Efficient Dispatch
    if (state == DataState::ASCENDING) {
        method = "None (Already Ascending)";
    }
    else if (state == DataState::DESCENDING) {
        method = "std::reverse (O(N))";
        std::reverse(data.begin(), data.end());
    }
    else {
        if (N < 256) {
            method = "Insertion Sort";
            insertionSort(data);
        } else if (N < 200000) {
            method = "Sequential Radix (8-bit)";
            radixSort<8, false>(data);
        } else if (N < 10000000) {
            method = "Parallel Radix (8-bit)";
            radixSort<8, true>(data);
        } else {
            method = "Parallel Radix (11-bit)";
            radixSort<11, true>(data);
        }
    }

    auto t_sort = high_resolution_clock::now();

    if (!data.empty()) {
        cout << "\n------------------------------------" << endl;
        cout << "First element: " << data.front() << endl;
        cout << "Last element:  " << data.back() << endl;
        cout << "------------------------------------" << endl;
    }

    if (argc >= 3) {
        string dest = argv[2];
        FILE* out_fp = (dest == "stdout") ? stdout : fopen(dest.c_str(), "wb");

        if (out_fp) {
            // --- ONE-SHOT MASSIVE I/O ---
            // Максимальная длина int32_t с минусом и '\n' - это 12 байт (-2147483648\n).
            // Выделяем один огромный буфер под все элементы сразу.
            size_t max_req_size = data.size() * 12;
            std::unique_ptr<char[]> giant_buffer(new char[max_req_size]);
            char* ptr = giant_buffer.get();

            for (int32_t v : data) {
                if (v == 0) {
                    *ptr++ = '0';
                } else {
                    uint32_t uval;
                    if (v < 0) {
                        *ptr++ = '-';
                        // Безопасный модуль без переполнения на INT_MIN
                        uval = ~static_cast<uint32_t>(v) + 1;
                    } else {
                        uval = static_cast<uint32_t>(v);
                    }

                    // Конвертация в обратном порядке
                    char temp[12];
                    int t_idx = 0;
                    while (uval > 0) {
                        temp[t_idx++] = '0' + (uval % 10);
                        uval /= 10;
                    }

                    // Copy the correct order to a giant buffer
                    while (t_idx > 0) {
                        *ptr++ = temp[--t_idx]; // Fixef from wptr on ptr
                    }
                }

                // Если выводим в терминал (stdout) - ставим пробел
                // Если в файл - оставляем '\n'
                *ptr++ = (out_fp == stdout) ? ' ' : '\n'; // Fixef from wptr on ptr
            }

            // Финальный перенос строки для терминала
            if (out_fp == stdout) {
                *ptr++ = '\n'; // Fixef from wptr on ptr
            }

            // Вычисляем итоговый размер данных и записываем всё ЗА 1 РАЗ
            size_t total_bytes = ptr - giant_buffer.get();
            fwrite(giant_buffer.get(), 1, total_bytes, out_fp);

            if (out_fp != stdout) {
                fclose(out_fp);
            }
        } else {
            cerr << "Error: Cannot open output file.\n";
        }
    }

    auto t_end = high_resolution_clock::now();
    double sort_ms = duration<double, milli>(t_sort - t_start).count();
    double total_ms = duration<double, milli>(t_end - t_start).count();

    cerr << fixed << setprecision(4)
         << "N: " << N << "\nMethod: " << method << "\nThreads: " << omp_get_max_threads()
         << "\nSorting time: " << sort_ms << " ms\n"
         << "Total time (with I/O): " << total_ms << " ms\n";
         printPeakMemory();
    return 0;
}
