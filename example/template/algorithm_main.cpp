#include <algorithm>
#include <vector>
#include <iostream>

template <typename T>
void sort_desc(std::vector<T>& arr) {
    std::sort(arr.begin(), arr.end(), [](auto i, auto j) {
        return i > j;
    });
}

int main() {
    std::vector<int> arr = {1, 2, 3, 4};
    sort_desc(arr);
    for (const auto& item : arr) {
        std::cout << item << ", ";
    }
}