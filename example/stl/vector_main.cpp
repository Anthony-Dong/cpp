#include <iostream>
#include <vector>
#include <list>
#include <fmt/core.h>

struct TestA {
    TestA() {
    }

    explicit TestA(int num) : num_(num) {
        std::cout << "TestA(" << num_ << ")\n";
    }

    ~TestA() {
        if (num_ == 0) {
            return;
        }
        std::cout << "~TestA(" << num_ << ")\n";
    }

    TestA(const TestA &a) = delete;

    TestA(TestA &&a) = delete;

    TestA &operator=(TestA &&a) noexcept {
        this->num_ = a.num_;
        a.num_ = 0;
        return *this;
    }

    int num_;
};

int main() {
    std::vector<TestA *> arr(10);
    arr.push_back(new TestA(1));

    std::list<TestA> list_;
    fmt::println("hello world");
}