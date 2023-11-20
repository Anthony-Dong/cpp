#include "cpp/network/listener.h"
#include "cpp/network/utils.h"
#include "cpp/network/task_queue.h"
#include <unistd.h>
#include <fmt/core.h>
#include <event2/event.h>
#include "cpp/utils/os.h"
#include "cpp/log/logger.h"
#include "cpp/network/exception.h"

using namespace cpp::network;

struct Connection {
    explicit Connection(int socket_fd, sockaddr_in addr) : fd_(socket_fd), addr_(addr) {
        INFO("new conn {} {}", utils::IPV4(this->addr_), fd_);
    }

    ~Connection() {
        if (fd_) {
            INFO("delete conn {} {}", utils::IPV4(this->addr_), fd_);
            ::close(fd_);
            fd_ = 0;
        }
    }

    size_t Read(char *data, size_t size) const {
        return ::read(fd_, data, size);
    }

    size_t Write(char *data, size_t size) const {
        return ::write(fd_, data, size);
    }

private:
    sockaddr_in addr_{};
    int fd_{};
};

int main() {
    fmt::print("pid: {}\n", cpp::utils::GetPID());
    utils::TaskQueue taskQueue{};
    auto listener = listener::NewTCPListener(8888, false);
    auto echoHandler = [](const Connection &conn) {
        const size_t buffer_size = 64;
        char buffer[buffer_size];
        while (true) {
            // 1. 同步 读取数据
            auto read_num = conn.Read(buffer, buffer_size);
            if (read_num == 0) { // EOF
                return;
            }
            if (read_num < 0) {
                throw exception::ErrNoException("read find error");
            }
            // 2. 同步 写出数据
            conn.Write(buffer, read_num);
            if (std::string_view(buffer, 4) == "exit") {
                return;
            }
        }
    };
    while (true) {
        // 主线程负责监听端口+获取连接
        sockaddr_in conn_addr{};
        socklen_t conn_addr_size = sizeof(conn_addr);
        auto conn_fd = accept(listener, (sockaddr *)&conn_addr, &conn_addr_size);
        if (conn_fd == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            throw exception::ErrNoException("accept find error");
        }
        if (conn_fd == 0) {
            //            fmt::print("READ FD find err: {}\n", strerror(errno));
            continue;
        }
        // 子线程负责去处理r/w等逻辑
        taskQueue.AddTask([=]() {
            Connection conn(conn_fd, conn_addr);
            echoHandler(conn);
        });
    }
}
