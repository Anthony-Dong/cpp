#include <iostream>
#include <type_traits>
#include <string>
#include <concepts>

template <typename T>
struct Stringer {
    Stringer() = delete;
    std::string to_string(T t) = delete;
};

template <typename T>
concept has_to_string_func = requires(T t) {
    // {expression} noexcept(optional) -> type-constraint;
    // 本质上就是，std::is_same_v<decltype(std::declval<Stringer<T>>().to_string(std::declval<T>())), std::string>
    { t.to_string() } -> std::same_as<std::string>;
};

template <typename T>
concept is_stringer = requires(T t) {
    // {expression} noexcept(optional) -> type-constraint;
    requires requires(Stringer<T> s) {
        { s.to_string(t) } -> std::same_as<std::string>;
    };

    // 也可以这么写，不过推荐上面这种写法
    //    { std::declval<Stringer<T>>().to_string(t) } -> std::same_as<std::string>;
};

template <has_to_string_func T>
inline std::string to_string(T t) {
    return t.to_string();
}

template <is_stringer T>
inline std::string to_string(T t) {
    return (Stringer<T>{}).to_string(t);
}

template <typename T>
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
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