#include "utils.h"

namespace cpp::http::utils {

std::string build_http_request(const std::string& method, const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::string output{};
    static std::string CRLF = "\r\n";

    auto to_upper = [](const std::string& str) {
        std::string result = str;
        std::transform(str.begin(), str.end(), result.begin(), ::tolower);
        return str;
    };

    output.append(to_upper(method));
    output.append(" ");
    output.append(url);
    output.append(" ");
    output.append("HTTP/1.1");
    output.append(CRLF);

    for (const auto& header : headers) {
        output.append(header.first);
        output.append(": ");
        output.append(header.second);
        output.append(CRLF);
    }

    if (!body.empty()) {
        output.append("Content-Length");
        output.append(": ");
        output.append(std::to_string(body.size()));
        output.append(CRLF);
    } else {
        output.append("Content-Length: 0");
        output.append(CRLF);
    }

    output.append(CRLF);
    output.append(body);

    if (!body.empty()) {
        output.append(CRLF);
    }
    return output;
}

} // namespace cpp::http::utils