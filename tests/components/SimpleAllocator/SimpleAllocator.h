//
// Created by dmitriiart on 2/23/25.
//

#ifndef FIXEDSIZEALLOCATOR_H
#define FIXEDSIZEALLOCATOR_H

#include <limits>
#include <iostream>


namespace trees::testing {

struct SimpleAllocatorCounter {
    inline static int count = 0;
};

template <typename T>
struct SimpleAllocator {
    using value_type = T;
    template <typename U>
    struct rebind {
        using other = SimpleAllocator<U>;
    };

    explicit SimpleAllocator() = default;

    template <typename U>
    explicit SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        if (n > std::numeric_limits<size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        SimpleAllocatorCounter::count += n;
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        std::free(p);
        SimpleAllocatorCounter::count -= n;
    }
};

template <typename T, typename U>
bool operator==(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return false; }

}


#endif //FIXEDSIZEALLOCATOR_H
