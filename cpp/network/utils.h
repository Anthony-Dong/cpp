#pragma once
#include <string>

#include "header.h"

namespace cpp::network::utils {

inline std::string IPV4(sockaddr_in &ipv4) {
    std::string output;
    output.append(inet_ntoa(ipv4.sin_addr));
    output.append(":");
    output.append(std::to_string(htons(ipv4.sin_port)));
    return output;
}

} // namespace cpp::network::utils