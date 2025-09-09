#include <iostream>

template <typename T, size_t N>
class AreaAllocator {
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;

    AreaAllocator() : buffer(operator new (sizeof(value_type) * N))
    {}

    ~AreaAllocator() {
        operator delete(buffer);
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

    void reset() {
        offset = 0; 
    }

private:
    size_type capacity = N;
    size_type offset = 0;
    pointer buffer;

};