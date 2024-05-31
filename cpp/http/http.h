#pragma once

#include "io.h"
#include "parser.h"
#include "utils.h"
#include <string>
#include <utility>
#include <vector>

#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"

namespace cpp::http {
using socket = asio::ip::tcp::socket;
using socket_ptr = std::shared_ptr<asio::ip::tcp::socket>;
using connection_ptr = std::shared_ptr<io::connection>;
using headers_t = std::map<std::string, std::vector<std::string>>;

struct request {
    [[nodiscard]] std::string method() const {
        return method_;
    }
    void set_method(const std::string& method) {
        method_ = method;
    }

    [[nodiscard]] std::string url() const {
        return url_;
    }
    void set_url(const std::string& url) {
        url_ = url;
    }

    [[nodiscard]] const headers_t& headers() const {
        return headers_;
    }
    void set_headers(const headers_t& headers) {
        this->headers_ = headers;
    }

    [[nodiscard]] const std::shared_ptr<io::reader>& body() const {
        return body_;
    }
    void set_body(std::shared_ptr<io::reader> body) {
        body_ = std::move(body);
    }

    [[nodiscard]] std::string version() const {
        return version_;
    }
    void set_version(const std::string& version) {
        version_ = version;
    }

    std::string& buffer() {
        return buffer_;
    }

    [[nodiscard]] int conten_length() const {
        return conten_length_;
    }
    void set_conten_length(int conten_length) {
        conten_length_ = conten_length;
    }

    [[nodiscard]] const connection_ptr& conn() const {
        return conn_;
    }

    void set_conn(const connection_ptr& conn) {
        conn_ = conn;
    }

    void reset() {
        version_.clear();
        method_.clear();
        url_.clear();
        headers_.clear();
        body_.reset();
        buffer_.clear();
        conten_length_ = 0;
        conn_.reset();
    }

    std::string version_{};
    std::string method_{};
    std::string url_{};
    headers_t headers_{};
    int conten_length_{};
    std::shared_ptr<io::reader> body_{};
    connection_ptr conn_;
    std::string buffer_{};
};

struct response {
    [[nodiscard]] int status_code() const {
        return status_code_;
    }
    void set_status_code(int status_code) {
        status_code_ = status_code;
    }
    [[nodiscard]] const headers_t& headers() const {
        return headers_;
    }
    void set_headers(const headers_t& headers) {
        headers_ = headers;
    }
    [[nodiscard]] const std::shared_ptr<io::reader>& body() const {
        return body_;
    }
    void set_body(std::shared_ptr<io::reader> body) {
        body_ = std::move(body);
    }
    std::string& buffer() {
        return buffer_;
    }
    [[nodiscard]] int conten_length() const {
        return conten_length_;
    }

    void set_conten_length(const int conten_length) {
        conten_length_ = conten_length;
    }

    void reset() {
        status_code_ = 0;
        headers_.clear();
        conten_length_ = 0;
        body_.reset();
        buffer_.clear();
    }

    int status_code_{};
    headers_t headers_{};
    int conten_length_{};
    std::shared_ptr<io::reader> body_{};
    std::string buffer_{};
};

template <typename Parser>
struct http_body_reader final : io::reader {
    using parser_ptr = std::shared_ptr<Parser>;
    http_body_reader(connection_ptr conn, parser_ptr parser) : connection_(std::move(conn)), parser_(std::move(parser)) {
        assert(connection_);
        assert(parser_);
    }
    async<size_t> read_some(char* buffer, size_t buffer_size) override {
        if (parser_->is_message_complete()) {
            co_return parser_->read_body(buffer, buffer_size);
        }
        const auto read_size = co_await connection_->read_some(buffer, buffer_size);
        parser_->parse(buffer, read_size);
        co_return parser_->read_body(buffer, buffer_size);
    }
    async<void> close() {
        if (parser_.is_message_complete()) {
            co_return;
        }
        utils::discard_reader(this);
    }

private:
    connection_ptr connection_;
    parser_ptr parser_;
};

using request_parser_t = parser::http_request_parser<request>;
using request_parser_ptr = std::shared_ptr<request_parser_t>;
using http_handle_func = std::function<async<void>(const request& req, response& resp)>;
namespace utils {
async<void> write_response(const connection_ptr& conn, response& response);

async<void> read_request(const connection_ptr& conn, const request_parser_ptr& parser, request& request);

// must be copy socket_ptr
asio::awaitable<void> handle_http_conn(socket_ptr socket, const http_handle_func& http_handler);
} // namespace utils

inline asio::awaitable<void> handler_listener(asio::ip::tcp::acceptor& listener, const http_handle_func& http_handler) {
    for (;;) {
        auto conn = co_await listener.async_accept(asio::use_awaitable);
        asio::co_spawn(listener.get_executor(), utils::handle_http_conn(std::make_shared<socket>(std::move(conn)), http_handler), asio::detached);
        // todo fix 这里没有捕获到异常...
    }
}

} // namespace cpp::http