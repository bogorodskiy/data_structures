#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <exception>
#include "RawMemory.h"

namespace ds
{
    template <typename T>
    class Vector
    {
    public:
        // TODO: remove test
        int id = 0;

        explicit Vector(size_t capacity = 1) : _data(capacity)
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

        Vector& operator = (const Vector& source)
        {
            auto resizeRequired = source._count > _data.getCapacity();
            if (resizeRequired)
            {
                Vector sourceCopy(source);
                swap(sourceCopy);
                return *this;
            }

            // copy elements until min count
            for (size_t minIndex = 0; minIndex < _count && minIndex < source._count; ++minIndex)
            {
                _data[minIndex] = source._data[minIndex];
            }

            // copy remainig elements
            if (_count < source._count)
            {
                auto copyFrom = source._data.getDataPtr() + _count;
                auto copyTo = _data.getDataPtr() + _count;
                auto elementsCount = source._count - _count;
                std::uninitialized_copy_n(copyFrom, elementsCount, copyTo);
            }
            // delete extra elements
            else if (_count > source._count)
            {
                auto deleteFrom = _data.getDataPtr() + source._count;
                auto elementsCount = _count - source._count;
                std::destroy_n(deleteFrom, elementsCount);
            }

            return *this;
        }

        Vector& operator = (Vector&& target) noexcept
        {
            std::cout << "operator = (Vector&& target), this = " << id << ", target = " << target.id << std::endl;
            swap(target);
            target._count = 0;
            return *this;
        }

        // iterators
        T* begin() 
        {
            return _data.getDataPtr();
        }
        T* end()
        {
            return _data.getDataPtr() + _count;
        }

        size_t getCount() const
        {
            return _count;
        }

        size_t getCapacity() const
        {
            return _data.getCapacity();
        }
        
        void setResizeFactor(size_t value)
        {
            _resizeFactor = value;
        }

        void pushBack(const T& element)
        {
            prepareForNewElement();

            new (_data + _count) T(element);
            _count++;
        }

        void pushBack(T&& element)
        {
            prepareForNewElement();

            new (_data + _count) T(std::move(element));
            _count++;
        }
        
        template <typename ... Args>
        T& emplaceBack(Args&&... args)
        {
            prepareForNewElement();

            auto result = new (_data + _count) T(std::forward<Args>(args)...);
            _count++;
            return *result;
        }

        void popBack()
        {
            if (_count == 0)
            {
                throw std::exception("Unable to popBack from vector, vector is empty");
            }

            std::destroy_n(_data.getDataPtr() + _count - 1, 1);
            _count--;
        }

        void clear()
        {
            std::destroy_n(_data.getDataPtr(), _count);
            _data.clear();
            _count = 0;
        }

        void reserve(const size_t capacity)
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
                std::uninitialized_copy_n(_data.getDataPtr(), _count, newData.getDataPtr());
            }
            else
            {
                std::uninitialized_move_n(_data.getDataPtr(), _count, newData.getDataPtr());
            }

            std::destroy_n(_data.getDataPtr(), _count);
            _data.swap(newData);
        }

    private:
        size_t _count = 0;
        RawMemory<T> _data;
        size_t _resizeFactor = 2;

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

        void prepareForNewElement()
        {
            if (_count == _data.getCapacity())
            {
                auto newCapacity = (_count == 0) ? 1 : _count * _resizeFactor;
                reserve(newCapacity);
            }
        }
    };
}

