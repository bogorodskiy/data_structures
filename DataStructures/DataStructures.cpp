// DataStructures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "Vector.h"
#include "Map.h"

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
    
    ds::Map<int, int> map;
    map.add(0, 10);
    map.add(1, 11);
    map.add(2, 12);
    map.add(3, 13);
    map.add(4, 14);
    map.add(5, 15);
    map.add(6, 16);
    map.add(7, 17);
    map.add(8, 18);
    map.add(9, 19);

    map.printRaw();



    map[3] = 777;
    std::cout << map[3] << std::endl;

    std::cout << map.contains(4) << std::endl;
    std::cout << map.contains(-1) << std::endl;

    map.removeAll();

    std::vector<int> vec;
    vec.begin();
}