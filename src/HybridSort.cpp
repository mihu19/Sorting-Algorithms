#include "utils.h" 
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <omp.h>
using namespace std;

//ALIGNED MEMORY – portable across MSVC, GCC, Clang, MinGW
#include <cstdlib>
#if defined(_MSC_VER)
  #include <malloc.h>
  inline void* portable_aligned_alloc(size_t alignment, size_t size) { return _aligned_malloc(size, alignment); }
  inline void  portable_aligned_free(void* p) { _aligned_free(p); }
#elif defined(_WIN32) && !defined(_MSC_VER)
  //for Mingw
  #include <malloc.h>
  inline void* portable_aligned_alloc(size_t alignment, size_t size) {
      void* raw = std::malloc(size + alignment + sizeof(void*));
      if (!raw) return nullptr;
      void* aligned = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(raw) + alignment + sizeof(void*)) & ~(alignment - 1));
      reinterpret_cast<void**>(aligned)[-1] = raw;
      return aligned;
  }
  inline void portable_aligned_free(void* p) {
      if (p) std::free(reinterpret_cast<void**>(p)[-1]);
  }
#else
  //Linux, macOS
  inline void* portable_aligned_alloc(size_t alignment, size_t size) {
      void* p = nullptr;
      return posix_memalign(&p, alignment, size) == 0 ? p : nullptr;
  }
  inline void portable_aligned_free(void* p) { std::free(p); }
#endif
#define ALLOC(s) portable_aligned_alloc(64, (s))
#define FREE(p)  portable_aligned_free(p)

//HELPER CLASSES AND FUNCTIONS
enum class DataState { ASCENDING, DESCENDING, RANDOM };

DataState checkDataState(const vector<int>& arr) {
    size_t n = arr.size();
    if (n < 2) return DataState::ASCENDING;

    bool is_asc = true, is_desc = true;
    for (size_t i = 1; i < n; ++i) {
        if (arr[i] < arr[i - 1]) is_asc = false;
        if (arr[i] > arr[i - 1]) is_desc = false;
        if (!is_asc && !is_desc) return DataState::RANDOM;
    }
    return is_asc ? DataState::ASCENDING : DataState::DESCENDING;
}

template<typename T> struct AlignedDel { void operator()(T* p) const { FREE(p); } };
template<typename T> using AlignedPtr = unique_ptr<T[], AlignedDel<T>>;
template<typename T> AlignedPtr<T> alloc_aligned(size_t n) { return AlignedPtr<T>(static_cast<T*>(ALLOC(n * sizeof(T)))); }

//CORE RADIX SORT ALGORITHM
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

void hybridSort(vector<int>& data) {
    size_t N = data.size();
    if (N < 2) return;

    DataState state = checkDataState(data);

    if (state == DataState::ASCENDING) 
        return; 
    else if (state == DataState::DESCENDING) 
        std::reverse(data.begin(), data.end());
    else {
        if (N < 256) {
            insertionSort(data); // uses the one from utils.h
        } else if (N < 200000) {
            radixSort<8, false>(data);
        } else if (N < 10000000) {
            radixSort<8, true>(data);
        } else {
            radixSort<11, true>(data);
        }
    }

}

