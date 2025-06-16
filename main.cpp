#include <absl/time/time.h>
#include <fmt/base.h>
#include <fmt/core.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/json/json.h>
#include <gsl/gsl>
#include <gsl/pointers>
#include <spdlog/spdlog.h>
#include <string>

struct TestStruct {
    explicit TestStruct(gsl::owner<std::string*> str) : str_ptr(str) {
    }

    ~TestStruct() {
        delete str_ptr;
    }

    const std::string* get_str_ptr() const {
        return str_ptr;
    }

private:
    gsl::owner<std::string*> str_ptr;
};

int main() {
    spdlog::info("hello {}", "cmake");
    SPDLOG_INFO("hello {}", "cmake");
    SPDLOG_INFO("hello {} {}", "world", "!");

    SPDLOG_INFO("hello world");

    constexpr absl::Duration ten_ns = absl::Nanoseconds(100000000000);
    SPDLOG_INFO(absl::ToInt64Seconds(ten_ns));

    fmt::println("hello world");

    using namespace google::protobuf;

    std::string out;
    Any any;
    auto status = json::MessageToJsonString(any, &out);
    fmt::println("status {}", static_cast<int>(status.code()));

    gsl::owner<std::string*> str = new std::string("11111");

    TestStruct test(str);
    fmt::println("str: {}", *test.get_str_ptr());
    return 0;
}