#include <iostream>

#include <map>

struct Test {
    int x;
    int y;
};

int main() {
    std::map<int, int> map;
    auto &&[itr, inserted] = map.insert({1, 2});
    if (inserted) {
        std::cout << "inserted" << std::endl;
        std::cout << "{" << itr->first << ", " << itr->second << "}" << std::endl;
    }
    for (auto &&[k, v] : map) {
        std::cout << "{" << k << ", " << v << "}" << std::endl;
    }

    Test t{.x = 2, .y = 1};

    auto &&[x, _] = t;
}