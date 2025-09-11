#include <iostream>
#include <vector>

template <typename T>
class DebugAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    DebugAllocator() = default;

    pointer allocate(size_type n) {
        pointer p = static_cast<pointer>(operator new (sizeof(value_type) * n));
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

    void deallocate(pointer p, size_type n) {
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

int main() {

    std::vector<int, DebugAllocator<int>> vec;
    vec.push_back(3);

    vec.reserve(3);
    vec.push_back(4);


    std::cout << vec[0] << vec[1] << "\n";
    return 0;
}
