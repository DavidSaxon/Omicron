// TODO: REMOVE ME
#include <iostream>

#include "omicron/Engine.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello Omicron!" << std::endl;

    return omi::Engine::get_instance()->execute();
}
