#include "omicron_runtime/Engine.hpp"

int main(int argc, char* argv[])
{
    return omi_::Engine::get_instance()->execute();
}
