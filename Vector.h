#pragma once

#include <iostream>

class Test {
public:

	int data = 0;

	Test() {
		std::cout << "Test ctor" << std::endl;
	}

	Test(const Test& source) {
		std::cout << "Test copy ctor" << std::endl;
	}

	Test(const Test&& source) {
		std::cout << "Test move ctor" << std::endl;
	}

	Test& operator= (const Test& target) {
		std::cout << "Test assignment operator" << std::endl;
		return *this;
	}

	Test& operator= (Test&& target) {
		std::cout << "Test move assignment operator" << std::endl;
		return *this;
	}

	~Test() {
		std::cout << "Test dtor" << std::endl;
	}
};

template <typename T>
class Vector {
public:
	Vector() {
		// TODO: allocate m_array on stack memory
		std::cout << "Vector ctor" << std::endl;
		m_array = new T[DEFAULT_SIZE];
		m_capacity = DEFAULT_SIZE;
		m_size = 0;
	}

	Vector(const Vector<T>& source) {
		std::cout << "Vector ctor" << std::endl;
		copyVector(source);
	}

	Vector(Vector<T>&& source) {
		std::cout << "Vector ctor" << std::endl;
		copyVector(source);
		source.destroy();
	}

	Vector<T>& operator= (const Vector<T>& target) {
		std::cout << "Vector assignment operator" << std::endl;
		delete[] m_array;
		copyVector(target);
		return *this;
	}

	Vector<T>& operator= (Vector<T>&& target) {
		std::cout << "Vector move assignment operator" << std::endl;
		delete[] m_array;
		copyVector(target);
		target.destroy();
		return *this;
	}

	T& const operator[](size_t index) const{
		std::cout << "Square brackets const ref [] get operator" << std::endl;
		if (index >= 0 && index < m_size) {
			return m_array[index];
		}
		throw std::out_of_range("index out of range");
	}

	T& operator[](size_t index) {
		std::cout << "Square brackets reference [] get operator" << std::endl;
		if (index >= 0 && index < m_size) {
			return m_array[index];
		}
		throw std::out_of_range("index out of range");
	}

	void pushBack(T& element) {
		if (m_size == m_capacity) {
			reallocate();
		}
		std::cout << "Set new element at " << m_size << std::endl;
		m_array[m_size] = element;
		std::cout << " ---" << std::endl;
		++m_size;
	}

	void popBack() {
		if (m_size > 0) {
			--m_size;
		}
	}

	size_t getSize() const{
		return m_size;
	}

	~Vector() {
		std::cout << "Vector dtor" << std::endl;
		destroy();
	}
private:

	static const int DEFAULT_SIZE = 3;

	void copyVector(const Vector<T>& source) {
		m_capacity = source.m_capacity;
		m_size = source.m_size;
		m_array = new T[m_capacity];
		
		for (auto i = 0; i < source.m_capacity; ++i) {
			m_array[i] = source.m_array[i];
		}
	}

	void reallocate() {
		m_capacity *= 2;
		std::cout << "Reallocate, new capacity = " << m_capacity << std::endl;

		auto newArray = new T[m_capacity];
		for (auto i = 0; i < m_size; ++i) {
			newArray[i] = m_array[i];
		}
		delete[] m_array;
		std::cout << "Old array deleted = " << m_capacity << std::endl;
		m_array = newArray;
	}

	void destroy() {
		m_capacity = 0;
		if (m_array != nullptr) {
			delete[] m_array;
		}
		m_array = nullptr;
	}

	size_t m_capacity;
	size_t m_size;
	T* m_array;
};

template<typename T, size_t const Size>
class VectorIterator{
public:
	typedef VectorIterator self_type;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef std::random_access_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	explicit VectorIterator(pointer ptr, size_t const index) : 
		ptr(ptr),
		index(index) {
		
	}
private:
	pointer ptr;
	size_t index;

	bool compatible(self_type const& other) const {
		return ptr == other.ptr;
	}
};