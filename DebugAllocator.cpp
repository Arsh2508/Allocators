    #include <iostream>

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

    int main() {
    DebugAllocator<int> allocator;

    // Allocate memory for 5 integers
    int* arr = allocator.allocate(5);

    // Construct integers
    for (int i = 0; i < 5; ++i) {
        allocator.construct(&arr[i], i * 10);
    }

    std::cout << "Values:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Destroy integers
    for (int i = 0; i < 5; ++i) {
        allocator.destroy(&arr[i]);
    }

    // Deallocate memory
    allocator.deallocate(arr);

    return 0;
}
