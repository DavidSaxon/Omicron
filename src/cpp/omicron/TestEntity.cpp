#include "omicron/TestEntity.hpp"

#include <iostream>

TestEntity::TestEntity()
{
}

TestEntity::~TestEntity()
{
}

void TestEntity::update()
{
    std::cout << "cxx update!" << std::endl;
}
