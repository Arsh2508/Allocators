#include <iostream>

template <typename T, size_t N>
class AreaAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    AreaAllocator() : buffer(static_cast<pointer>(::operator new (sizeof(value_type) * N)))
    {}

    ~AreaAllocator() {
        ::operator delete(buffer);
    }   

    pointer allocate(size_type n) {
        if(offset + n <= capacity) {
            pointer ptr = buffer + offset;
            offset += n;
            return ptr;
        }
        else {
            return nullptr;
        }
    }
    
    template <typename... Args>
    void construct(pointer p, Args&&... args) {
        new (p) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        p->~value_type();
    }

    void deallocate(pointer p, size_type n) noexcept {
        // no-op
    }

    void reset() {
        offset = 0; 
    }

private:
    size_type capacity = N;
    size_type offset = 0;
    pointer buffer;

};

int main() {
    // Create an allocator for 10 integers
    AreaAllocator<int, 10> allocator;

    // Allocate memory for 5 integers
    int* arr = allocator.allocate(5);
    if (!arr) {
        std::cerr << "Allocation failed\n";
        return 1;
    }

    // Construct the integers
    for (int i = 0; i < 5; ++i) {
        allocator.construct(&arr[i], i * 10); // construct with value i*10
    }

    // Print the values
    std::cout << "Allocated and constructed values:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Destroy the integers
    for (int i = 0; i < 5; ++i) {
        allocator.destroy(&arr[i]);
    }

    // Reset allocator for reuse
    allocator.reset();

    // Allocate again to check reset
    int* arr2 = allocator.allocate(3);
    if (!arr2) {
        std::cerr << "Allocation failed after reset\n";
        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        allocator.construct(&arr2[i], i + 100);
    }

    std::cout << "Values after reset and new allocation:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << arr2[i] << " ";
    }
    std::cout << "\n";

    // Destroy new integers
    for (int i = 0; i < 3; ++i) {
        allocator.destroy(&arr2[i]);
    }

    int register a = 8;

    return 0;
}
