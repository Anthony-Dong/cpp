# cpp 注意事项

## 禁止在头文件中定义 非内联函数定义

函数定义与申明

将非内联函数的定义放在头文件中会导致以下问题： （from gpt）

- 重复定义：如果多个源文件包含了这个头文件，会导致函数的重复定义，从而引起链接错误
- 编译效率：头文件被多次包含时，函数的定义会被多次编译，这会降低编译效率
- 代码可读性：将非内联函数的定义放在头文件中，会使代码变得难以阅读和理解，不符合良好的代码风格

因此，建议将非内联函数的定义放在源文件中，并在头文件中声明。如果需要将函数的声明和定义放在一起，可以将它们放在同一个源文件中。

1. bad (原因是重复定义)

```cpp
// utils.h

#pragma once
#include <string>
namespace test::utils {
    std::string version(){
        return "1.0.0";
    }
}
```

2. good

```cpp
// utils.h
#pragma once
#include <string>
namespace test::utils {
    inline std::string version(){
        return "1.0.0";
    }
}
```

## 模版函数定义必须头文件中定义

模版需要实例化，因此必须要在头文件中定义！

## 头文件namespace 和 header 的处理

1. good

```cpp
#pragma once

#include <string>

#if __cplusplus >= 201703L
#include <string_view>
#endif // __cplusplus >= 201703L

namespace cpp::utils { # namespace 内部不要引入头文件，不然下面的头文件会继承定义的namespace
    std::string base64_encode(std::string_view s, bool url = false);
}
```

2. bad

```cpp
#pragma once

namespace cpp::utils { # namespace 内部不要引入头文件，不然下面的头文件会继承定义的namespace
#include <string>

#if __cplusplus >= 201703L
#include <string_view>
#endif // __cplusplus >= 201703L

    std::string base64_encode(std::string_view s, bool url = false);
}
```

## `std::string` 和 `\0` 的问题

std::string 的`append`/`+=`/`+`等函数会特殊处理 char_size，这里我们可以看下`operator+`的实现

```cpp
template<class _CharT, class _Traits, class _Allocator>
inline
basic_string<_CharT, _Traits, _Allocator>
operator+(const basic_string<_CharT, _Traits, _Allocator>& __lhs, const _CharT* __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__lhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __lhs_sz = __lhs.size();
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = _Traits::length(__rhs); # 会通过这个方法去获取length
    __r.__init(__lhs.data(), __lhs_sz, __lhs_sz + __rhs_sz);
    __r.append(__rhs, __rhs_sz);
    return __r;
}

// string的定义 的 _Traits是 char_traits<char>
typedef basic_string<char, char_traits<char>, allocator<char> > string;

// char_traits<char>::length 最终是这个实现，发现使用了strlen 因此如果是'\0'的话会有问题
length(const char_type* __s)  _NOEXCEPT {return __builtin_strlen(__s);}
```

**正确做法** ：在使用char*(cstring)的时候一定要指定`size_t`

```cpp
#include <string>
#include <iostream>

int str_append() {
    std::string str = "hello";
    str.push_back('\0');    // good
    str.append("a\0bc", 4); // good
    str = str + "a\0bc";    // bad, 这里会错误的处理成   str = str + "a";

    std::cout << str << std::endl; // hello a bca
}
```