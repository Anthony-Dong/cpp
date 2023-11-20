#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cassert>
#include <unistd.h>
#include <chrono>
#include "cpp/network/exception.h"
#include <event2/event.h>
#include <arpa/inet.h>
#include <thread>

int main() {
    using namespace std::chrono;
    using namespace cpp::network;
    using clock = std::chrono::system_clock;

    // Protocol family (domain)
    // SOCK (socket)
    // IP PROTO (protocol)
    auto conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(conn != -1);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10011);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    auto start = clock::now();
    if (connect(conn, (sockaddr *)&addr, sizeof(addr)) == -1) {
        throw exception::ErrNoException("new connection find err");
    }

    std::cout << "spend: " << duration_cast<microseconds>(clock::now() - start).count() << "us"
              << "\n";

    std::string data = "hello world";
    std::cout << write(conn, data.data(), data.size()) << std::endl;
    std::this_thread::sleep_for(seconds(10000));
    close(conn);
}