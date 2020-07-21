// DataStructures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "Vector.h"

class Thrower
{
public:
    int a;

    Thrower(int a)
    {
        this->a = a;
        std::cout << "thrower ctor" << std::endl;
    }
    Thrower(const Thrower& source)
    {
        std::cout << "thrower copy ctor" << std::endl;
        a = source.a;
    }
    Thrower(Thrower&& source) noexcept
    {
        std::cout << "thrower move ctor" << std::endl;
        this->a = source.a;
        source.a = 0;
    }
    ~Thrower()
    {
        std::cout << "thrower dtor" << std::endl;
    }
};

int main()
{
    ds::Vector<Thrower> v1(1);
    v1.id = 1;
    
    //v1.emplaceBack(1);
    //v1.pushBack(1);
    v1.emplaceBack(Thrower(11));
    v1.emplaceBack(Thrower(111));
    v1.emplaceBack(Thrower(1111));
    v1.clear();
    //v1.emplaceBack(Thrower(11111));

    //std::vector<Thrower> v2;
    //v2.reserve(3);
    //v2.emplace_back(1);
    //v2.push_back(1);
}