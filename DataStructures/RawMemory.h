#pragma once
#include <memory>
#include <utility>

namespace ds
{
    template <typename T>
    struct RawMemory
    {
    public:
        RawMemory(size_t capacity = InitialCapacity)
        {
            _data = allocate(capacity);
            _capacity = capacity;
        }

        RawMemory(const RawMemory& source) = delete;

        RawMemory& operator = (const RawMemory&) = delete;

        RawMemory& operator = (RawMemory&& target)
        {
            swap(target);
            target._capacity = 0;
            return *this;
        }

        T* operator + (size_t index)
        {
            return _data + index;
        }

        const T* operator + (size_t index) const
        {
            return _data + index;
        }

        T& operator [] (size_t index)
        {
            return _data[index];
        }

        const T& operator [] (size_t index) const
        {
            return _data[index];
        }

        size_t getCapacity() const
        {
            return _capacity;
        }

        T* getDataPtr()
        {
            return _data;
        }

        void swap(RawMemory& target) noexcept
        {
            std::swap(_data, target._data);
            std::swap(_capacity, target._capacity);
        }

        void clear()
        {
            deallocate(_data);
            
            _data = allocate(InitialCapacity);
            _capacity = InitialCapacity;
        }

        ~RawMemory()
        {
            deallocate(_data);
        }

    private:
        const int InitialCapacity = 1;
        T* _data = nullptr;
        size_t _capacity = 0;

        static T* allocate(size_t numElements)
        {
            return static_cast<T*>(operator new(numElements * sizeof(T)));
        }

        static void deallocate(T* memoryPtr)
        {
            operator delete(memoryPtr);
        }
    };
}