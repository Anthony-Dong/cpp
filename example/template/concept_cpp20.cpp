#include <concepts>
#include <type_traits>
#include <iostream>
#include <cassert>

// go 里面我们定义了一个 ReadWriter
// type Reader interface {
//	Read(p []byte) (n int, err error)
// }
// type Writer interface {
//	Write(p []byte) (n int, err error)
// }
// ReadWriter is the interface that groups the basic Read and Write methods.
// type ReadWriter interface {
//	Reader
//	Writer
//}

// 在C++中我们可以定义一个 concept来实现， 是不是非常牛逼哇！！！
template <typename T>
concept Reader = requires(T t) {
    requires requires(std::string& buffer) {
        { t.Read(buffer) } -> std::same_as<size_t>;
    };
};

template <typename T>
concept Writer = requires(T t) {
    requires requires(std::string& buffer) {
        { t.Write(buffer) } -> std::same_as<size_t>;
    };
};

template <typename T>
concept ReadWriter = Reader<T> && Writer<T>;



// 简单实现一个StringBuffer
struct StringBuffer {
    explicit StringBuffer(std::string&& buffer) : buffer_(std::move(buffer)) {
        w = buffer_.size();
        r = 0;
    };

    size_t Read(std::string& buffer) {
        auto size = buffer.size();
        if (w == r) {
            return 0;
        }
        if (w - r < size) {
            size = w - r;
        }
        std::copy(buffer_.begin() + long(r), buffer_.begin() + long(r) + long(size), buffer.begin());
        r = r + size;
        return size;
    }

    size_t Write(std::string& buffer) {
        buffer_.append(buffer, 0, buffer.size());
        w = w + buffer.size();
        return buffer.size();
    }

    size_t Write(std::string&& buffer) {
        return Write(buffer);
    }

private:
    size_t r, w;
    std::string buffer_;
};

// 实现ioutil.ReadAll(reader) 方法
template <ReadWriter Rw>
size_t ReadAll(Rw& reader, std::string& buffer) {
    buffer.clear();
    std::string bw{};
    bw.resize(16);
    while (true) {
        if (auto size = reader.Read(bw); size >= 0) {
            if (size == 0) {
                break;
            }
            buffer.append(bw, 0, size);
        }
    }
    return buffer.size();
}

int main() {
    StringBuffer sb("C++是世界上最好的语言");
    sb.Write("!!!!!");
    std::string buffer{};
    ReadAll(sb, buffer);
    std::cout << buffer << std::endl;
}