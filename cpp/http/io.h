#pragma once

#include "asio/ip/tcp.hpp"
#include "common.h"
#include <chrono>
#include <exception>
#include <string>
#include <utility>

namespace cpp::http::io {

struct reader {
    reader() = default;
    virtual ~reader() = default;
    virtual async<size_t> read_some(char* buffer, size_t buffer_size) {
        co_return 0;
    }
};

struct writer {
    writer() = default;
    virtual ~writer() = default;
    virtual async<size_t> write(char* buffer, size_t buffer_size) {
        co_return 0;
    }
};

struct io_exception final : std::exception {
    explicit io_exception(std::string msg) : msg_(std::move(msg)) {
    }
    [[nodiscard]] const char* what() const noexcept override {
        return this->msg_.c_str();
    }

private:
    std::string msg_;
};

struct connection final : reader, writer {
    using socket_ptr = std::shared_ptr<asio::ip::tcp::socket>;
    using steady_clock = std::chrono::steady_clock;
    explicit connection(socket_ptr socket) : socket_(std::move(socket)) {
        assert(socket_);
    }
    connection(connection& conn) = delete;

    [[nodiscard]] steady_clock::duration read_timeout() const {
        return read_timeout_;
    }

    void set_read_timeout(const steady_clock::duration& read_timeout) {
        read_timeout_ = read_timeout;
    }
    async<size_t> read_some(char* buffer, size_t buffer_size) override;

    async<size_t> write(char* buffer, size_t buffer_size) override;

    [[nodiscard]] asio::ip::tcp::endpoint local() const {
        return socket_->local_endpoint();
    }

    [[nodiscard]] asio::ip::tcp::endpoint remote() const {
        return socket_->remote_endpoint();
    }

private:
    steady_clock::duration read_timeout_{};
    // steady_clock::time_point write_timeout_;
    socket_ptr socket_;
};

} // namespace cpp::http::io