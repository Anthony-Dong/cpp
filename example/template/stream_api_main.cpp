#include <string>
#include <functional>
#include <vector>

template <typename T>
struct ArrayStream {
    explicit ArrayStream(const std::vector<T>& arr) : array_(arr) {
    }
    explicit ArrayStream(std::vector<T>&& arr) : array_(std::move(arr)) {
    }
    template <typename R>
    ArrayStream<R> Map(const std::function<R(const T&)>& mapper) {
        std::vector<R> ret;
        ret.reserve(array_.size());
        for (const auto& item : this->array_) {
            ret.push_back(mapper(item));
        }
        return ArrayStream<R>(std::move(ret));
    };

    ArrayStream<T> Filter(const std::function<bool(const T&)>& filter) {
        std::vector<T> ret;
        for (const auto& item : this->array_) {
            if (filter(item)) {
                ret.push_back(item);
            }
        }
        return ArrayStream<T>(std::move(ret));
    };

    std::vector<T> Move() {
        return std::move(array_);
    };

private:
    std::vector<T> array_{};
};

struct UserInfo {
    std::string name;
    int age;
};

int test() {
    std::vector<UserInfo> userinfos = {
        {"shiwu", 8},
        {"xiaoming", 10},
        {"xiaoli", 12},
    };
    auto names = ArrayStream<UserInfo>(userinfos)
                     .Filter([](const auto& item) {
                         return item.age > 10;
                     })
                     .Map<std::string>([](const auto& item) {
                         return item.name;
                     })
                     .Move();

    //    for (const auto& name : names) {
    ////        std::cout << "年龄大于10的用户: " << name << "\n";
    //    }
    return 0;
}

int main() {
    for (int x = 0; x < 100000; x++) {
        test();
    }
}