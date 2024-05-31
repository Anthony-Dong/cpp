#pragma once
#include "common.h"
#include <concepts>
#include <map>
#include <sstream>

namespace cpp::http::utils {

template <typename T>
concept is_reader_t = requires(T* t) {
                          requires requires(char* buffer, const size_t buffer_size) {
                                       { t->read_some(buffer, buffer_size) } -> std::same_as<async<size_t>>;
                                   };
                      };

template <is_reader_t T>
async<void> read_all(T* reader, std::string& output) {
    output.clear();
    char buffer[1024]{};
    for (;;) {
        const auto read_size = co_await reader->read_some(buffer, 1024);
        if (read_size == 0) {
            co_return;
        }
        output.append(buffer, read_size);
    }
}

template <is_reader_t T>
async<void> discard_reader(T* reader) {
    char buffer[1024]{};
    for (;;) {
        if (const auto read_size = co_await reader->read_some(buffer, 1024); read_size == 0) {
            co_return;
        }
    }
}

inline std::string to_lower(const std::string& key) {
    std::string output(key);
    std::transform(key.begin(), key.end(), output.begin(), [](auto c) {
        return std::tolower(c);
    });
    return output;
}

inline std::string num_to_hex(const size_t size) {
    std::stringstream ss{};
    ss << std::hex << size;
    return ss.str();
};

std::string build_http_request(const std::string& method, const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body);

} // namespace cpp::http::utils
