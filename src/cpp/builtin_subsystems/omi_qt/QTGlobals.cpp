#include "omi_qt/QTGlobals.hpp"

namespace global
{

arclog::Input* input = nullptr;

} // namespace global

arclog::Input*& logger = global::input;
