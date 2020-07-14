#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <memory>

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

    template <typename T>
    class Vector
    {
    public:
        // TODO: remove test
        int id = 0;

        explicit Vector(size_t capacity) : _data(capacity)
        {
        }

        Vector(const Vector& source) : _data(source._data)
        {
            std::uninitialized_copy_n(source._data.getDataPtr(), source.getCapacity(), _data.getDataPtr());
            _count = source._count;
        }

        Vector(Vector&& target)
        {
            swap(target);
        }

        ~Vector()
        {
            std::destroy_n(_data.getDataPtr(), _count);
        }

        Vector& operator = (Vector&& target)
        {
            std::cout << "operator = (Vector&& target), this = " << id << ", target = " << target.id << std::endl;
            swap(target);
            target._count = 0;
            return *this;
        }

        size_t getCount() const
        {
            return _count;
        }

        size_t getCapacity() const
        {
            return _data.getCapacity();
        }
        
        void pushBack(T&& element)
        {
            if (_data.getCapacity() <= _size)
            {
                
            }
        }

        void reserve(size_t capacity)
        {
            if (_data.getCapacity() >= capacity)
            {
                return;
            }

            RawMemory<T> newData(capacity);

            // T move ctor is no except

            constexpr bool tMoveCtorThrows = !noexcept(T(std::declval<T>()));
            if constexpr (tMoveCtorThrows)
            {
                std::cout << "reserve copy" << std::endl;
                std::uninitialized_copy_n(_data.getDataPtr(), _count, newData.getDataPtr());
            }
            else
            {
                std::cout << "reserve move" << std::endl;
                std::uninitialized_move_n(_data.getDataPtr(), _count, newData.getDataPtr());
            }

            std::destroy_n(_data.getDataPtr(), _count);
            _data.swap(newData);
        }

    private:
        size_t _count = 0;
        RawMemory<T> _data;

        static void Construct(void* data)
        {
            new (data) T();
        }

        static void Construct(void* data, const T& source)
        {
            new (data) T(source);
        }

        static void Construct(void* data, T& target)
        {
            new (data) T(std::move(target));
        }

        static void Destroy(T* data)
        {
            data->~T();
        }

        void swap(Vector& target) noexcept
        {
            _data.swap(target._data);
            std::swap(_count, target._count);
        }
    };
}

