#pragma once
#include <memory>
#include <utility>

namespace ds
{
    template <typename T>
    struct RawMemory
    {
    public:
        RawMemory(size_t numElements)
        {
            _data = allocate(numElements);
            _capacity = numElements;
        }

        RawMemory(const RawMemory& source) = delete;

        RawMemory& operator = (const RawMemory&) = delete;

        RawMemory& operator = (RawMemory&& target)
        {
            swap(target);
            target._capacity = 0;
            return *this;
        }

        const T* operator + (size_t i) const
        {
            return _data + i;
        }

        const T& operator [] (size_t i) const
        {
            return _data[i];
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

        ~RawMemory()
        {
            deallocate(_data);
        }

    private:
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