#pragma once

#include <string>

namespace cpp::network::listener {

std::string BevEventToString(short what);
int NewTCPListener(int port, bool isNio);

} // namespace cpp::network::listener