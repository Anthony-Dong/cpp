#pragma once

#include <stdexcept>
#include <string>
#include <cstring>

namespace cpp::network::exception {

struct ErrNoException : std::runtime_error {
    explicit ErrNoException(const char *name) : std::runtime_error(std::string(name) + ": " + strerror(errno)){};
};

struct CloseException : std::runtime_error {
    using std::runtime_error::runtime_error;
};
} // namespace cpp::network::exception