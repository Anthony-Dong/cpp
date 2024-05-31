#pragma once

#include "common.h"
#include "llhttp.h"
#include <map>
#include <string>
#include <utility>

namespace cpp::http {

namespace parser {

struct parser_exception : http_exception {
    using http_exception::http_exception;
};

using headers_t = std::map<std::string, std::vector<std::string>>;

template <typename T>
struct http_parser {
    void parse(const char* buffer, const size_t buffer_size) {
        // llhttp_resume(&this->parser_);
        auto errcode = llhttp_execute(&this->parser_, buffer, buffer_size);
        if (errcode == HPE_OK) {
            return;
        }
        throw parser_exception(llhttp_errno_name(errcode));
    }

    void init_parser(const std::function<void(llhttp_t&, llhttp_settings_t& settings)>& handle) {
        llhttp_settings_init(&this->parser_settings_);
        handle(this->parser_, this->parser_settings_);
    }

    void reset() {
        llhttp_reset(&parser_);
    }

private:
    llhttp_t parser_{};
    llhttp_settings_t parser_settings_{};
};

template <typename T>
struct http_header_parser {
    void init_llhttp_settings_t(llhttp_settings_t& settings) {
        settings.on_header_field = on_header_field;
        settings.on_header_value = on_header_value;
        settings.on_version = on_version;
        settings.on_header_field_complete = on_header_field_complete;
        settings.on_header_value_complete = on_header_value_complete;
        settings.on_headers_complete = on_headers_complete;
        settings.on_version_complete = on_version_complete;
    }

    static int on_version_complete(llhttp_t* parser) {
        const auto http_parser = static_cast<T*>(parser->data);
        const auto parser_data = static_cast<http_header_parser*>(http_parser);
        http_parser->set_version(parser_data->version_buffer_);
        return 0;
    }

    static int on_headers_complete(llhttp_t* parser) {
        const auto http_parser = static_cast<T*>(parser->data);
        const auto parser_data = static_cast<http_header_parser*>(http_parser);
        http_parser->set_headers(std::move(parser_data->headers_));
        http_parser->set_conten_length(parser->content_length);
        parser_data->is_headers_complete_ = true;
        return 0;
    }

    static int on_header_field_complete(llhttp_t* parser) {
        const auto parser_data = static_cast<http_header_parser*>(static_cast<T*>(parser->data));
        const auto& header_values = parser_data->headers_[parser_data->header_filed_buffer_];
        parser_data->header_field_ = parser_data->header_filed_buffer_;
        parser_data->header_filed_buffer_.clear();
        return 0;
    }

    static int on_header_value_complete(llhttp_t* parser) {
        const auto parser_data = static_cast<http_header_parser*>(static_cast<T*>(parser->data));
        auto& header_values = parser_data->headers_[parser_data->header_field_];
        header_values.push_back(parser_data->header_value_buffer_);
        parser_data->header_value_buffer_.clear();
        return 0;
    }

    static int on_header_field(llhttp_t* parser, const char* data, size_t size) {
        const auto parser_data = static_cast<http_header_parser*>(static_cast<T*>(parser->data));
        parser_data->header_filed_buffer_.append(data, size);
        return 0;
    }

    static int on_header_value(llhttp_t* parser, const char* data, size_t size) {
        const auto parser_data = static_cast<http_header_parser*>(static_cast<T*>(parser->data));
        parser_data->header_value_buffer_.append(data, size);
        return 0;
    }

    static int on_version(llhttp_t* parser, const char* data, size_t size) {
        const auto parser_data = static_cast<http_header_parser*>(static_cast<T*>(parser->data));
        parser_data->version_buffer_.append(data, size);
        return 0;
    }

    [[nodiscard]] bool is_headers_complete() const {
        return is_headers_complete_;
    }

    void reset() {
        this->headers_.clear();
        this->header_filed_buffer_.clear();
        this->header_value_buffer_.clear();
        this->version_buffer_.clear();
        this->header_field_.clear();
        this->is_headers_complete_ = false;
    }

private:
    headers_t headers_{};
    std::string header_filed_buffer_{};
    std::string header_value_buffer_{};
    std::string version_buffer_{};
    std::string header_field_{};
    bool is_headers_complete_{};
};

template <typename T>
struct http_body_parser {
    void init_llhttp_settings_t(llhttp_settings_t& settings) {
        settings.on_body = on_body;
        // settings.on_chunk_extension_name = on_chunk_extension_name;
        // settings.on_chunk_extension_value = on_chunk_extension_value;
        settings.on_chunk_header = on_chunk_header;
        settings.on_chunk_complete = on_chunk_complate;
        settings.on_message_complete = on_message_complete;
    }

    static int on_message_complete(llhttp_t* parser) {
        const auto req = static_cast<http_body_parser*>(static_cast<T*>(parser->data));
        req->is_message_complete_ = true;
        return 0;
    }

    // "6\r\n123456\r\n" + "7\r\n1234567\r\n" + "0\r\n\r\n"
    // on_chunk_header(6) -> on_body -> on_chunk_complate
    // on_chunk_header(7) -> on_body -> on_chunk_complate
    // on_chunk_header(0) -> on_chunk_complate
    static int on_chunk_header(llhttp_t* parser) {
        // std::cout << "on_chunk_header\n";
        return 0;
    }

    static int on_chunk_complate(llhttp_t* parser) {
        //  std::cout << "on_chunk_complate\n";
        return 0;
    }

    static int on_body(llhttp_t* parser, const char* data, size_t size) {
        const auto req = static_cast<http_body_parser*>(static_cast<T*>(parser->data));
        req->body_buffer_.append(data, size);
        return 0;
    }

    size_t read_body(char* buffer, size_t buffer_size) {
        const size_t read_size = buffer_size <= body_buffer_.size() ? buffer_size : body_buffer_.size();
        std::copy_n(body_buffer_.begin(), read_size, buffer);
        if (read_size == body_buffer_.size()) {
            body_buffer_.clear();
            return read_size;
        }
        body_buffer_ = std::string(body_buffer_.begin() + static_cast<long>(read_size), body_buffer_.end());
        return read_size;
    }

    [[nodiscard]] bool is_message_complete() const {
        return is_message_complete_;
    }

    void reset() {
        this->is_message_complete_ = false;
        this->body_buffer_.clear();
    }

private:
    std::string body_buffer_{};
    // std::map<std::string, std::string> chunked_headers_{};
    bool is_message_complete_{};
};

struct http_response_parser : http_parser<http_response_parser>, http_header_parser<http_response_parser>, http_body_parser<http_response_parser> {
    http_response_parser() {
        init_llhttp_settings_t();
    }
    void init_llhttp_settings_t() {
        init_parser([this](llhttp_t& parser, llhttp_settings_t& settings) {
            http_header_parser::init_llhttp_settings_t(settings);
            http_body_parser::init_llhttp_settings_t(settings);
            settings.on_status = on_status;
            settings.on_status_complete = on_status_complete;
            llhttp_init(&parser, HTTP_RESPONSE, &settings);
            parser.data = this;
        });
    }

    static int on_status_complete(llhttp_t* parser) {
        const auto parser_data = static_cast<http_response_parser*>(parser->data);
        parser_data->status_code_ = parser->status_code;
        return 0;
    }

    static int on_status(llhttp_t* parser, const char* data, const size_t size) {
        const auto parser_data = static_cast<http_response_parser*>(parser->data);
        parser_data->status_.append(data, size);
        return 0;
    }

    [[nodiscard]] std::string status() const {
        return status_;
    }
    [[nodiscard]] int status_code() const {
        return status_code_;
    }
    void set_headers(const headers_t& t) {
    }
    void set_version(const std::string& version) {
    }
    void set_conten_length(int conten_length) {
    }

private:
    std::string status_{};
    int status_code_{};
};

template <typename T>
struct http_request_parser : http_parser<http_request_parser<T>>, http_header_parser<http_request_parser<T>>, http_body_parser<http_request_parser<T>> {
    using http_parser_t = http_parser<http_request_parser<T>>;
    using http_header_parser_t = http_header_parser<http_request_parser<T>>;
    using http_body_parser_t = http_body_parser<http_request_parser<T>>;
    http_request_parser() {
        init_llhttp_settings_t();
    }
    void reset(T* request) {
        this->request_ = request;
        this->method_buffer_.clear();
        this->url_buffer_.clear();
        http_parser_t::reset();
        http_header_parser_t::reset();
        http_body_parser_t::reset();
    }

    void init_llhttp_settings_t() {
        http_parser<http_request_parser<T>>::init_parser([this](llhttp_t& parser, llhttp_settings_t& settings) {
            http_header_parser_t::init_llhttp_settings_t(settings);
            http_body_parser_t::init_llhttp_settings_t(settings);
            settings.on_method = on_method;
            settings.on_url = on_url;
            settings.on_method_complete = on_method_complete;
            settings.on_url_complete = on_url_complete;
            llhttp_init(&parser, HTTP_REQUEST, &settings);
            parser.data = this;
        });
    }

    static int on_method_complete(llhttp_t* parser) {
        const auto parser_data = static_cast<http_request_parser*>(parser->data);
        if (parser_data->request_) {
            parser_data->request_->set_method(parser_data->method_buffer_);
        }
        return 0;
    }

    static int on_url_complete(llhttp_t* parser) {
        const auto parser_data = static_cast<http_request_parser*>(parser->data);
        if (parser_data->request_) {
            parser_data->request_->set_url(parser_data->url_buffer_);
        }
        return 0;
    }

    static int on_method(llhttp_t* parser, const char* data, const size_t size) {
        const auto req = static_cast<http_request_parser*>(parser->data);
        req->method_buffer_.append(data, size);
        return 0;
    }

    static int on_url(llhttp_t* parser, const char* data, size_t size) {
        const auto req = static_cast<http_request_parser*>(parser->data);
        req->url_buffer_.append(data, size);
        return 0;
    }

    void set_headers(const headers_t& t) {
        if (request_) {
            request_->set_headers(t);
        }
    }

    void set_version(const std::string& version) {
        if (request_) {
            request_->set_version(version);
        }
    }
    void set_conten_length(const int conten_length) {
        if (request_) {
            request_->set_conten_length(conten_length);
        }
    }

private:
    std::string method_buffer_{};
    std::string url_buffer_{};
    T* request_{};
};

} // namespace parser

} // namespace cpp::http