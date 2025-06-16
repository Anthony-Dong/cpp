//
// Created by bytedance on 2024/4/22.
//
#include "http.h"
#include "spdlog/spdlog.h"

template <>
struct fmt::formatter<asio::ip::tcp::endpoint> : fmt::formatter<std::string_view> {
     static auto format(const asio::ip::tcp::endpoint& endpoint, fmt::format_context& ctx)  {
        return fmt::format_to(ctx.out(), "[{}]:{}", endpoint.address().to_string(), endpoint.port());
    }
};

namespace cpp::http::utils {
async<void> write_response(const connection_ptr& conn, response& response) {
    std::string& output = response.buffer();
    if (output.capacity() == 0) {
        output.reserve(1024);
    }
    static std::string CRLF = "\r\n";
    static std::string chunkedCRLF = "0\r\n\r\n";
    if (response.status_code() == 0) {
        output.append("HTTP/1.1 200 OK");
        output.append(CRLF);
    } else {
        output.append("HTTP/1.1 ");
        output.append(std::to_string(response.status_code()));
        output.append(" OK");
        output.append(CRLF);
    }
    // write header
    for (const auto& [key, values] : response.headers()) {
        for (const auto& value : values) {
            output.append(key);
            output.append(":");
            output.append(value);
            output.append(CRLF);
        }
    }

    if ((response.conten_length() > 0 || response.conten_length() == -1) && response.body() == nullptr) {
        throw http_exception("body为空");
    }
    // write content length
    if (response.conten_length() >= 0) {
        output.append("Content-Length");
        output.append(":");
        output.append(std::to_string(response.conten_length()));
        output.append(CRLF);
    }
    const bool is_chunked = response.conten_length() == -1;
    if (is_chunked) {
        output.append("Transfer-Encoding: chunked");
        output.append(CRLF);
    }
    // write header endl
    output.append(CRLF);

    // flush header
    co_await conn->write(output.data(), output.size());

    if (response.conten_length() == 0) {
        co_return;
    }

    if (output.size() < 1024) {
        output.resize(1024);
    }
    auto& resp_body = response.body();
    if (resp_body == nullptr) {
        co_return;
    }
    // chunked: 6\r\n123456\r\n0\r\n\r\n
    // 其他: 123456
    for (;;) {
        const auto read_size = co_await resp_body->read_some(output.data(), output.size());
        if (read_size == 0) {
            if (is_chunked) {
                co_await conn->write(chunkedCRLF.data(), chunkedCRLF.size());
            }
            co_return;
        }
        if (is_chunked) {
            auto chunk_size_header = utils::num_to_hex(read_size);
            chunk_size_header.append(CRLF);
            co_await conn->write(chunk_size_header.data(), chunk_size_header.size());
        }
        co_await conn->write(output.data(), read_size);
        if (is_chunked) {
            co_await conn->write(CRLF.data(), CRLF.size());
        }
    }
}

async<void> read_request(const connection_ptr& conn, const request_parser_ptr& parser, request& request) {
    auto& buffer = request.buffer();
    if (buffer.size() < 1024) {
        buffer.resize(1024);
    }
    char* buffer_data = buffer.data();
    const size_t buffer_size = buffer.size();
    for (;;) {
        const auto read_size = co_await conn->read_some(buffer_data, buffer_size);
        parser->parse(buffer_data, read_size);
        if (parser->is_headers_complete()) {
            request.set_conn(conn);
            request.set_body(std::make_shared<http_body_reader<request_parser_t>>(conn, parser));
            co_return;
        }
    }
}

async<void> handle_http_conn(socket_ptr socket, const http_handle_func& http_handler) {
    using namespace std::chrono_literals;

    const auto& conn = std::make_shared<io::connection>(socket);
    conn->set_read_timeout(60s); // todo set timeout

    const auto& request_parser = std::make_shared<parser::http_request_parser<request>>();
    request request{};
    response response{};
    request_parser->reset(&request);
    try {
        for (;;) {
            co_await read_request(conn, request_parser, request);
            co_await http_handler(request, response);
            co_await write_response(conn, response);

            request.reset();
            response.reset();
            request_parser->reset(&request);
        }
    } catch (const parser::parser_exception& err) {
        SPDLOG_INFO("{} HTTP解析异常: {}", conn->remote(), err.what());
    } catch (const http_exception& err) {
        SPDLOG_INFO("HTTP异常: {}", err.what());
    } catch (const io::io_exception& err) {
        SPDLOG_INFO("IO异常: {}", err.what());
    } catch (const std::exception& err) {
        SPDLOG_INFO("系统异常: {}", err.what());
    } catch (...) {
        SPDLOG_INFO("未知异常");
        socket->close();
        throw;
    }
    socket->close();
}

} // namespace cpp::http::utils