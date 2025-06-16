#include <iostream>

struct A {
    A() = default;
    virtual ~A() = default;
    virtual std::string name() {
        return "A";
    };
};

struct B final : A {
    B() = default;
    ~B() override = default;
    std::string name() override {
        return "B";
    };
    size_t data_;
};

int main() {
    A a = B{};
    std::cout << a.name() << "\n"; // 输出A

    std::cout << &a << "\n";

    A* aa = new B{};
    std::cout << aa->name() << "\n"; // 输出B

    std::cout << "A: " << sizeof(A) << ", B: " << sizeof(B) << "\n"; // B的大小是16，原因是因为有一个指针指向了虚表

    delete aa;
}