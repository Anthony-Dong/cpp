#include <iostream>
#include <memory>
struct Test {
    explicit Test(std::string& name) : name(name) {
    }

    std::string& name;
};

struct Test2 {
    std::string& name;
};

struct Test3 {
    std::string name;
};

int main() {
    Test3 t3{.name = "小明"};
    auto test = std::make_shared<Test3>(std::move(t3));

    std::cout << test->name << "\n";

    std::cout << t3.name << "\n";
}
