#include <string>
#include <iostream>
#include "gtest/gtest.h"

TEST(StringTest, TestAppend) {
    std::string str = "hello";
    str.push_back('\0');    // good
    str.append("a\0bc", 4); // good
    str = str + "a\0bc";    // bad, 这里会错误的处理成   str = str + "a";

    std::cout << str << std::endl; // hello a bca
}

TEST(StringTest, TestCChar) {
    const char* str = "hello world\0hello world";
    std::cout << std::string(str).size() << std::endl;
}