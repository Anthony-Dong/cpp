#include <utility>

#include "spdlog/spdlog.h"

struct Parent {
    std::string name() {
        return name_;
    }
    explicit Parent(std::string name) : name_(std::move(name)) {
    }

private:
    std::string name_{};
};

struct Child : Parent {
    explicit Child(std::string name) : Parent(std::move(name)) {
    }
    std::string name() {
        return "hello world" + Parent::name();
    }
};

int main(int argc, char* argv[]) {
    Child child("哈哈哈哈");
    fmt::println("{}", child.name());
}
