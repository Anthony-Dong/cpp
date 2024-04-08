#include <iostream>
#include <type_traits>
#include <string>

template <typename T>
struct Stringer {
    Stringer() = delete;
    std::string to_string(T t) = delete;
};

template <typename T, typename = void>
struct is_stringer : std::false_type {};
template <typename T>
struct is_stringer<T, std::enable_if_t<std::is_same_v<decltype(std::declval<Stringer<T>>().to_string(std::declval<T>())), std::string>>> : std::true_type {};

template <typename T, typename = void>
struct has_to_string_func : std::false_type {};
template <typename T>
struct has_to_string_func<T, std::enable_if_t<std::is_same_v<decltype(std::declval<T>().to_string()), std::string>>> : std::true_type {};

template <typename T>
inline std::enable_if_t<has_to_string_func<T>::value, std::string> to_string(T t) {
    return t.to_string();
}

template <typename T>
inline std::enable_if_t<is_stringer<T>::value, std::string> to_string(T t) {
    return (Stringer<T>{}).to_string(t);
}

template <typename T>
inline std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, std::string> to_string(T t) {
    return std::to_string(t);
}

template <typename T>
inline std::enable_if_t<std::is_same_v<decltype(std::string(std::declval<T>())), std::string>, std::string> to_string(T t) {
    return t;
}

inline std::string to_string(bool b) {
    return b ? "true" : "false";
}

struct Test1 {
    std::string to_string() {
        return "Test1(" + std::string("name=") + name + ", f2=" + ::to_string(age) + ")";
    };
    std::string name;
    int age;
};

struct Test2 {
    std::string f1;
    int f2;
};

template <>
struct Stringer<Test2> {
    std::string to_string(const Test2& t) {
        return "Test2(" + std::string("f1=") + t.f1 + ", f2=" + ::to_string(t.f2) + ")";
    };
};

int main() {
    std::cout << to_string(Test1{.name = "tom", .age = 18}) << std::endl;
    std::cout << to_string(Test2{.f1 = "F1", .f2 = 2}) << std::endl;
    std::cout << to_string(false) << std::endl;
    std::cout << to_string(true) << std::endl;
    std::cout << to_string(1.11) << std::endl;
    std::cout << to_string(1111) << std::endl;
    std::cout << to_string("1111") << std::endl;
    std::cout << to_string("1111") << std::endl;
}

// output:
// Test1(name=tom, f2=18)
// Test2(f1=F1, f2=2)
// false
// true
// 1.110000
// 1111
// 1111