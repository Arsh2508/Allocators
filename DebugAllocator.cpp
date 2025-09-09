#include <iostream>

template <typename T>
class DebugAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    DebugAllocator() = default;

    pointer allocate(size_type n) {
        poineter p = operator new value_type(sizeof(value_type) * n);
        std::cout << "Memory allocated in: " << p << "\n"; 
        return p;
    }

    template <typename... Args>
    void construct(pointer p, Args&&... args) {
        new (p) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        std::cout << "Memory destroyed in: " << p << "\n";
        p->~value_type();
    }

    void deallocate(pointer p) {
        std::cout << "Memory deallocated in: " << p << "\n";
        operator delete(p);
    }

    template <typename U>
    bool operator==(const DebugAllocator<U>& other) {
        return true;
    }

    template <typename U>
    bool operator!=(const DebugAllocator<U>& other) {
        return false;
    }
};