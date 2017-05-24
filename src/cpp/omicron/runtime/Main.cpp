#include "omicron/runtime/Engine.hpp"


int main(int argc, char* argv[])
{
    return omi::runtime::Engine::get_instance()->execute();
}
