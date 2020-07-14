// DataStructures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vector.h"

class Thrower
{
public:
    int a;

    explicit Thrower(int a)
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
};

int main()
{
    ds::Vector<Thrower> v1(1);
    v1.id = 1;
    v1.reserve(3);
    
    ds::Vector<Thrower> v2(1);
    v2.id = 2;
    v1 = std::move(v2);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
