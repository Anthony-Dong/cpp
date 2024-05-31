#include <iostream>
#include <stdexcept>

void test() {
    throw std::runtime_error("异常");
}

void test2() {
    try {
        test();
    } catch (...) {
        throw;
    }
}

int main() {
    try {
        test2();
    } catch (const std::runtime_error& err) {
        std::cout << err.what() << "\n";
    }
}