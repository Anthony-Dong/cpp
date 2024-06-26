#include "gtest/gtest.h"
#include <iostream>
#include <string>

TEST(StringTest, TestAppend) {
    std::string str = "hello";
    str.push_back('\0');    // good
    str.append("a\0bc", 4); // good
    str = str + "a\0bc";    // bad, 这里会错误的处理成   str = str + "a";

    std::cout << str << std::endl; // hello a bca
}

template <class T>
class ConstSingleton {
public:
    /**
     * Obtain an instance of the singleton for class T.
     * @return const T& a reference to the singleton for class T.
     */
    static const T& get() {
        static T* instance = new T();
        return *instance;
    }
};

using Str = ConstSingleton<std::string>;

TEST(StringTest, ConstSingleton) {
    std::cout << Str::get() << std::endl;
}

TEST(StringTest, TestCChar) {
    const char* str = "hello world\0hello world";
    std::cout << std::string(str).size() << std::endl;
}

TEST(StringTest, TestCut) {
    std::string data("hello world");

    const char* str = data.c_str();

    std::cout << std::string(str, 4) << "\n";
}

struct reader {
    size_t read(char*, size_t);
};

struct writer {
    size_t write(char*, size_t);
};



TEST(StringTest, move) {
    std::map<std::string, std::vector<std::string>> order_map;
    {
        auto& value =  order_map["1"];
        value.emplace_back("1");
        value.emplace_back("2");
    }

    {
        auto& value =  order_map["2"];
        value.emplace_back("1");
        value.emplace_back("2");
    }

    for (const auto& [key, value] : order_map) {
        std::cout << key<<": ";

        for (auto& basic_string : value) {
            std::cout << basic_string<<", ";
        }
        std::cout <<"\n";
    }
}