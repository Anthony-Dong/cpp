#pragma once

#include "asio/awaitable.hpp"

namespace cpp::http {

template <typename T>
using async = asio::awaitable<T>;

struct http_exception : std::exception {
    explicit http_exception(std::string msg) : msg_(std::move(msg)) {
    }
    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

} // namespace cpp::http