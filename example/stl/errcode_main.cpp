#include <iostream>
#include <system_error>
#include <type_traits>

// 定义错误码
enum class MyErrc {
    NotEnoughMemory = 1,
    ActionTimedOut,
    SystemBusy
};

// 定义错误种类
class MyErrcCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "MyErrorCategory";
    }

    std::string message(int ev) const override {
        switch (static_cast<MyErrc>(ev)) {
        case MyErrc::NotEnoughMemory:
            return "Not enough memory";
        case MyErrc::ActionTimedOut:
            return "Action timed out";
        case MyErrc::SystemBusy:
            return "System too busy";
        default:
            return "Unknown error";
        }
    }

    // 用来创建一个对应错误码的error_condition对象
    std::error_condition default_error_condition(int ev) const noexcept override {
        return std::error_condition(ev, *this);
    }
};

const MyErrcCategory& myErrCategory() {
    static MyErrcCategory instance;
    return instance;
}

// 让系统知道MyErrc是一个错误码
namespace std {

template <>
struct is_error_code_enum<MyErrc> : true_type {};
} // namespace std

// 用来创建一个std::error_code对象
std::error_code make_error_code(MyErrc e) {
    static MyErrcCategory instance;
    return std::error_code(int(e), instance);
}

std::error_code test() {
    return std::error_code(MyErrc::ActionTimedOut);
}

int main() {
    auto err = test();
    if (err) {
        std::cout << err.category().name() << "\n";
    }

    std::cout << std::is_error_condition_enum<MyErrc>::value << "\n";
}