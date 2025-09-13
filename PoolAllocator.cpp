#include <exception>
#include <algorithm>
#include <iostream>

template <typename T>
class PoolAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    PoolAllocator(size_type _block_size, size_type _block_count) 
        : block_size{std::max(_block_size, sizeof(void*))}
        , block_count{_block_count}
        , allocated{0}
        , head{::operator new(block_size * block_count)}
    {
        char *start = static_cast<char *>(head);
        for(size_type i = 0; i < block_count; ++i) {
            void *current_block = start + (i * block_size);
            void *next_block = (i == block_count - 1) ? nullptr : start + ((i + 1) * block_size);

            *reinterpret_cast<void **>(current_block) = next_block;
        }

        free_head = head;
    }

    ~PoolAllocator() {
        ::operator delete(head);
    }

    pointer allocate() {
        if(is_fool()) {
            throw std::bad_alloc();
        }

        pointer ret = static_cast<pointer>(free_head);
        free_head = *reinterpret_cast<void **>(free_head);
        ++allocated;
        return ret;

    }

    bool is_fool() {
        return allocated == block_count;
    }

    void deallocate(pointer ptr) {
        *reinterpret_cast<void **>(ptr) = free_head;
        free_head = ptr;

        --allocated;
    }

    template <typename... Args>
    void construct(pointer ptr, Args&&... args) {
        new (ptr) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }

private:  
    size_type block_size;
    size_type block_count;
    size_type allocated = 0;
    void *head;
    void *free_head;
};


int main()
{
    PoolAllocator<int> pa(sizeof(int) * 2, 4);  

    int *ptr = pa.allocate();
    pa.construct(ptr, 3);


    int *ptr2 = pa.allocate();
    pa.construct(ptr2, 42);

    std::cout << *ptr << " " << *ptr2 << "\n";

    pa.destroy(ptr);
    pa.deallocate(ptr);

    pa.destroy(ptr2);
    pa.deallocate(ptr2);

}