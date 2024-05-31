#include "io.h"
#include "asio/as_tuple.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "asio/steady_timer.hpp"
#include "asio/use_awaitable.hpp"

namespace cpp::http::io {

async<size_t> connection::read_some(char* buffer, size_t buffer_size) {
    using namespace asio::experimental::awaitable_operators;
    if (this->read_timeout_.count() > 0) {
        asio::steady_timer timer(this->socket_->get_executor(), this->read_timeout_);
        const auto result = co_await (this->socket_->async_read_some(asio::buffer(buffer, buffer_size), asio::as_tuple(asio::use_awaitable)) || timer.async_wait(asio::use_awaitable));
        if (result.index() == 0) {
            auto [errcode, read_size] = std::get<0>(result);
            if (errcode) {
                throw io_exception("读异常[" + std::to_string(errcode.value()) + "]: " + errcode.message());
            }
            co_return read_size;
        }
        throw io_exception("读异常: 超时");
    }
    auto [errcode, result] = co_await this->socket_->async_read_some(asio::buffer(buffer, buffer_size), asio::as_tuple(asio::use_awaitable));
    if (errcode) {
        throw io_exception(errcode.message());
    }
    co_return result;
}

async<size_t> connection::write(char* buffer, size_t buffer_size) {
    const auto [errcode, result] = co_await this->socket_->async_send(asio::buffer(buffer, buffer_size), asio::as_tuple(asio::use_awaitable));
    if (errcode) {
        throw io_exception("写异常[" + std::to_string(errcode.value()) + "]: " + errcode.message());
    }
    co_return result;
}

} // namespace cpp::http::io