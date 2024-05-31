#include <iostream>
#include <map>

using headers_t = std::map<std::string, std::string>;

std::ostream& operator << (std::ostream& out, const headers_t& headers) {
    for ( const auto& header : headers) {
        std::cout << header.first << ": " << header.second << "\n";
    }
    return out;
}

template <typename T>
struct headers_parser {
    static void on_header(void* ptr, const std::string& key, const std::string& value) {
        auto parser = static_cast<headers_parser*>(static_cast<T*>(ptr));
        parser->headers_[key] = value;
    }
    headers_t headers_{};
};

template <typename T>
struct body_parser {
    static void on_data(void* ptr, const std::string& data) {
        auto parser = static_cast<body_parser*>(static_cast<T*>(ptr));
        parser->body_ = data;
    }
    std::string body_{};
    headers_t extendion_headers_{};
};

struct response_parser : virtual  headers_parser<response_parser>,virtual body_parser<response_parser> {
    std::string status_{};
    std::string version_{};
};

int main() {
    response_parser parser{};
    response_parser::on_data(&parser,"data");
    response_parser::on_header(&parser,"k1","v1");
    std::cout << parser.body_ << "\n";
    std::cout << parser.headers_ << "\n";
}
