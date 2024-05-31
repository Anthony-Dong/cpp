#include "cpp/http/http.h"
#include "spdlog/spdlog.h"
#include <unistd.h>

template <>
struct fmt::formatter<asio::ip::tcp::endpoint> : fmt::formatter<std::string_view> {
    static auto format(const asio::ip::tcp::endpoint& endpoint, fmt::format_context& ctx) {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

cpp::http::async<void> handler(const cpp::http::request& request, cpp::http::response& response) {
    // SPDLOG_INFO("receive http request. remote: {}, method: {}, url: {}, content-length: {}", request.conn()->remote(), request.method(), request.url(), request.conten_length());
    response.set_conten_length(request.conten_length());
    response.set_body(request.body());
    co_return;
}

int main() {
    SPDLOG_INFO("pid: {}", getpid());
    asio::io_context ctx;
    try {
        asio::ip::tcp::acceptor listener(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));
        asio::co_spawn(ctx, cpp::http::handler_listener(listener, handler), asio::detached);
        ctx.run();
    } catch (...) {
        ctx.stop();
        throw;
    }
}