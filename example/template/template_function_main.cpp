#include <iostream>
#include <type_traits>

// template <typename T>
// constexpr auto isStringer(T t) -> std::enable_if_t<std::is_same<decltype(std::declval<T>().String()), std::string>::value, bool> {
//     return true;
// }
//
// template <typename T>
// constexpr auto isStringer(...) -> bool {
//     return false;
// }




template <typename T, typename = void>
struct isStringer : std::false_type {};

template <typename T>
struct isStringer<T, std::void_t<decltype(std::declval<T>().String())>> : std::true_type {};

struct Stringer {
    std::string String() {
        return "hello world";
    };
};

struct Stringer2 {
    std::string String2() {
        return "hello world";
    };
};

int main() {
    bool x = isStringer<Stringer2>::value;
    bool y = isStringer<Stringer>::value;
}