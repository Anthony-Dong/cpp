#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <utility>

template <typename T>
concept is_std_to_string_v = requires(T t) {
    { std::to_string(t) } -> std::same_as<std::string>;
};

template <typename T>
    requires is_std_to_string_v<T>
inline std::string to_string(T t) {
    return std::to_string(t);
}

template <typename T>
    requires std::is_convertible_v<T, std::string>
inline std::string to_string(T t) {
    return t;
}

inline std::string to_string(bool b) {
    return b ? "true" : "false";
}

// template <typename T, typename... Args>
// std::common_type_t<T, Args...> min(T t, Args... args) {
//     return (t < args ? t:  args)
// }

// template <typename... Ts>
// auto min(Ts... args) {
//     return (std::min)({args...});
// }

template <typename A, typename B, typename C = std::common_type_t<A, B>>
C min(A a, B b) {
    return a < b ? a : b;
}



int main() {

}