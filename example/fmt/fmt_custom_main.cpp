#include "fmt/format.h"

#include <fstream>
#include <iostream>
#include <unistd.h>

struct Test {
    std::string str;
};

int main(int argc, char* argv[]) {

    std::ifstream stream("/Users/bytedance/go/src/github.com/anthony-dong/cpp/example/fmt/fmt_test.cpp");
    if (!stream) {
        std::cout << "error\n";
        return -1;
    }
    for (;;) {
        std::string buffer{};
        buffer.resize(1024);
        stream.read(buffer.data(), buffer.size());
        std::cout << buffer;
        if (stream.eof()) {
            break;
        }
    }

    stream.close();
}
