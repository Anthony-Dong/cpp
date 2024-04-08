#include "cpp/log/logger.h"
#include "cpp/network/listener.h"
#include "cpp/network/task_queue.h"
#include "cpp/network/utils.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/event.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace cpp::network;

void on_read(bufferevent* bev, void* ctx);
void on_event(bufferevent* bev, short what, void* ctx);

struct Connection {
    explicit Connection(event_base* base, evutil_socket_t fd, sockaddr_in addr) : base_(base), fd_(fd), addr_(addr) {
        // BEV_OPT_CLOSE_ON_FREE: 表示当free的时候会close掉socket
        bufferevent_ = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        assert(bufferevent_ != nullptr);
        // 设置读回掉，当有数据的时候回掉on_read函数
        bufferevent_setcb(bufferevent_, on_read, nullptr, on_event, this);
        bufferevent_enable(bufferevent_, EV_READ | EV_WRITE);
        INFO("new conn {} {}", utils::IPV4(this->addr_), fd_);
    }

    ~Connection() {
        if (fd_) {
            INFO("delete conn {} {}", utils::IPV4(this->addr_), fd_);
            bufferevent_free(this->bufferevent_);
            fd_ = 0;
        }
    }

public:
    void SetTimeOut(long read_timeout, long write_timeout) {
        timeval wtimeout_{.tv_sec = read_timeout, .tv_usec = 0};
        timeval rtimeout_{.tv_sec = write_timeout, .tv_usec = 0};
        bufferevent_set_timeouts(this->bufferevent_, &rtimeout_, &wtimeout_);
    }

private:
    friend void on_event(bufferevent* bev, short what, void* ctx);

    friend void on_read(bufferevent* bev, void* ctx);

private:
    event_base* base_;
    bufferevent* bufferevent_{};

    evutil_socket_t fd_;
    sockaddr_in addr_;
};

void on_read(bufferevent* bev, void* ctx) {
    auto conn = (Connection*)ctx;
    //        auto client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    auto buff = bufferevent_socket_new(conn->base_, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(buff, on_read, nullptr, on_event, conn);
    bufferevent_enable(buff, EV_READ | EV_WRITE);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10011);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    using namespace std::chrono;
    auto start = system_clock::now();
    if (auto status = bufferevent_socket_connect(buff, (sockaddr*)&addr, sizeof(addr)); status == -1) {
        std::cout << "conn find err:" << strerror(errno) << "\n";
    }
    std::cout << duration_cast<microseconds>((system_clock::now() - start)).count() << "us\n";
    //  fmt::print("{} [{}] {} on read\n", Utils::GetCurrentDate(), Utils::GetThreadID(), Utils::IPV4(conn->addr_));
    // 获取读/写 buffer
    auto read = bufferevent_get_input(bev);
    auto write = bufferevent_get_output(bev);
    // 往写buffer中写出即可
    assert(evbuffer_add_buffer(write, read) == 0);
}

void on_event(bufferevent* bev, short what, void* ctx) {
    auto conn = (Connection*)ctx;
    INFO("on event {} {}", utils::IPV4(conn->addr_), listener::BevEventToString(what));
    if (what & BEV_EVENT_EOF) { // 关闭连接的事件
        delete conn;
        return;
    }
    if (what & BEV_EVENT_ERROR) { // 异常事件
        delete conn;
        return;
    }
    if (what & BEV_EVENT_TIMEOUT) { // 读/写超时的事件
        delete conn;
        return;
    }
}

void runListener(int listener) {
    auto base = event_base_new();
    // 1. 创建一个listen event，持续监听连接事件
    auto listen_event = event_new(
        base, listener, EV_PERSIST | EV_READ,
        [](evutil_socket_t listener, short flag, void* arg) {
            auto* base = (event_base*)arg;
            if ((flag & EV_READ) == 0) {
                throw std::runtime_error(std::string("illegal event: ") + strerror(errno));
            }
            // 2. 当有连接时， accept 连接
            while (true) {
                sockaddr_in conn_addr{};
                socklen_t conn_addr_len = sizeof(conn_addr);
                auto conn_fd = accept(listener, (sockaddr*)&conn_addr, &conn_addr_len);
                if (conn_fd == -1) {
                    return;
                }
                // 3. 创建一个连接，并且启动监听读事件
                auto conn = new Connection(base, conn_fd, conn_addr);
                conn->SetTimeOut(5, 5);
            }
        },
        base);

    // 添加事件
    event_add(listen_event, nullptr);
    // 当前线程启动调度器
    event_base_dispatch(base);
    // 结束，释放资源
    event_free(listen_event);
    event_base_free(base);
}

int main() {

    //    fmt::print("pid: {}\n", Network::Utils::GetPID());
    cpp::network::utils::TaskQueue queue{};
    int threadNum = 4;
#ifdef __APPLE__
    threadNum = 1;
#endif
    for (int x = 0; x < threadNum; x++) {
        // Linux是支持一个端口多个进程/线程监听的
        auto listener = cpp::network::listener::NewTCPListener(8888, true);
        queue.AddTask([=]() {
            runListener(listener);
        });
    }
}
