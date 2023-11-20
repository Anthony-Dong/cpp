#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/signal.h>
#include <sys/wait.h>
// g++ -std=c++14 main.cpp -o main

int main() {
    std::cout << "main start: " << getpid() << "\n";
    auto child_pid = fork();
    if (child_pid < 0) {
        std::cout << "fork find err: " << strerror(errno) << "\n";
    }
    if (child_pid == 0) {
        // 子进程
        std::cout << "child pid " << getpid() << " start" << "\n";
        signal(SIGINT, [](int args) {
            std::cout << "child pid: " << "SIGINT" << "\n";
            return;
        });
        sleep(50);
        std::cout << "child pid " << getpid() << " end" << "\n";
        return 0;
    }
    std::cout << "main start wait child process success: " << child_pid << "\n";
    sleep(5);

//    if (kill(child_pid, SIGINT) == -1) {
//        std::cout << "main kill child process find err: " << strerror(errno) << "\n";
//    }

    if (waitpid(child_pid, nullptr, 0) != child_pid) { // 正确做法是分配有限的资源或者及时回收
        std::cout << "main wait child process find err: " << strerror(errno) << "\n";
        return 1;
    }
    std::cout << "main wait child process success: " << child_pid
              << "\n";
}