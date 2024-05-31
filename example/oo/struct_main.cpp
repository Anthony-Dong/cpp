#include <iostream>
#include <memory>

struct TestA {
    ~TestA() {
        std::cout << "~TestA()\n";
    }
};

int main() {
    auto x = std::make_shared<TestA>();
    {
        std::cout << "reset start\n";
        x.reset();
        std::cout << "reset end\n";
    }

    std::cout << "clear\n";
}
// ~TestD()
// ~TestC()
// ~TestB()
// ~TestA()