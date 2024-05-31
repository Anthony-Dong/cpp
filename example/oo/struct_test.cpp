#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

struct reader {
    reader() = default;
    ~reader() {
        SPDLOG_INFO("~reader");
    };
    virtual size_t read_some(char* buffer, size_t buffer_size) {
        return 0;
    };
};

struct c_reader : reader {
    c_reader() = default;
    ~c_reader() {
        SPDLOG_INFO("~c_reader");
    };
    virtual size_t read_some(char* buffer, size_t buffer_size) {
        return 2;
    };
};

struct buffer_reader final : c_reader {
    explicit buffer_reader(std::string& buffer) : buffer_(buffer) {
    }
    ~buffer_reader() {
        SPDLOG_INFO("~buffer_reader");
    }
    size_t read_some(char* buffer, size_t buffer_size) override {
        // auto size = buffer_size > buffer_.size() ? buffer_.size() : buffer_size;
        // std::copy_n(buffer_.begin(), size, buffer);
        return buffer_size;
    }

private:
    std::string& buffer_;
};

void test(reader& r) {
    SPDLOG_INFO(r.read_some(nullptr, 100));
}

TEST(Struct, oo) {
    std::string buffer = "hello world";
    { std::shared_ptr<std::string> ptr = std::make_shared<std::string>(buffer); }
    SPDLOG_INFO(buffer);
}




struct MyTest {
    MyTest() {
        SPDLOG_INFO("MyTest");
    }
    ~MyTest() {
        SPDLOG_INFO("~MyTest");
    }

    MyTest(const MyTest& my){
        SPDLOG_INFO("MyTest(MyTest&)");
    }

    MyTest(const MyTest&& my) noexcept {
        SPDLOG_INFO("MyTest(MyTest&&)");
    }
};

void test( const std::shared_ptr<MyTest>&  ptr) {
    std::cout << ptr.use_count() << "\n";
}

TEST(Struct, meme) {
    MyTest my;
    test(std::make_shared<MyTest>(std::move(my)));
}