#include "listener.h"
#include "header.h"
#include "exception.h"
#include "event2/bufferevent.h"

namespace cpp::network::listener {

int NewTCPListener(int port, bool isNio) {
    // #include <sys/socket.h>
    // domain: ipv4/ipv6/unix
    // socket: stream / dgram
    // protocol: 0/tcp/udp
    // https://man7.org/linux/man-pages/man2/socket.2.html
    int socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFD == -1) {
        throw exception::ErrNoException("new socket error");
    }

    if (isNio) {
        assert(evutil_make_socket_nonblocking(socketFD) == 0);
    }
    assert(evutil_make_listen_socket_reuseable(socketFD) == 0);
    assert(evutil_make_listen_socket_reuseable_port(socketFD) == 0);

    // #include <netinet/in.h>
    // https://man7.org/linux/man-pages/man2/bind.2.html
    // https://www.cnblogs.com/Long-w/p/9604201.html
    // port=8888
    // host="127.0.0.1" or INADDR_ANY
    // sin_family AF_INET
    // htons -> host to net (short 16位) port
    // htonl -> host to net (lang 32位)  ip
    sockaddr_in addr{}; // in: inet
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (auto status = bind(socketFD, (sockaddr *)&addr, sizeof(addr)); status == -1) {
        throw exception::ErrNoException("bind socket error");
    }

    // #include <sys/socket.h>
    // https://man7.org/linux/man-pages/man2/listen.2.html
    if (auto result = listen(socketFD, 512); result == -1) {
        throw exception::ErrNoException("listen socket error");
    }
    return socketFD;
}

std::string BevEventToString(short what) {
    auto output = std::string{};
    if (what & BEV_EVENT_CONNECTED) {
        output += "CONNECTED ";
    }
    if (what & BEV_EVENT_TIMEOUT) {
        output += "TIMEOUT ";
    }
    if (what & BEV_EVENT_ERROR) {
        output += "ERROR(";
        output += strerror(errno);
        output += ") ";
    }
    if (what & BEV_EVENT_EOF) {
        output += "EOF ";
    }
    if (what & BEV_EVENT_WRITING) {
        output += "WRITING ";
    }
    if (what & BEV_EVENT_READING) {
        output += "READING ";
    }
    return output;
}

} // namespace cpp::network::listener
