#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

namespace cpp::io {

FileReadWriter::FileReadWriter(std::string filename) {
    w_ = -1;
    r_ = -1;
    while (true) {
        file_descriptor_ = open(filename.data(), O_RDWR | O_CREAT | O_SYNC, 0644);
        if (file_descriptor_ < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::runtime_error(std::string("open file find err: ") + std::strerror(errno));
        }
        //  O_APPEND
        w_ = ::lseek(file_descriptor_, 0, SEEK_END);
        mod_ = W;
        break;
    }
}

FileReadWriter::~FileReadWriter() {
    if (file_descriptor_ > 0) {
        close(file_descriptor_);
        file_descriptor_ = 0;
    }
}

size_t FileReadWriter::read(std::string &buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (r_ == -1) {
        r_ = 0;
    }
    if (mod_ != Mode::R) {
        mod_ = Mode::R;
        ::lseek(file_descriptor_, r_, SEEK_SET);
    }
    auto size = ::read(file_descriptor_, buffer.data(), buffer.size());
    if (size > 0) {
        r_ += size;
    }
    return size;
}
size_t FileReadWriter::write(const std::string &buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (w_ == -1) {
        w_ = 0;
    }
    if (mod_ != Mode::W) {
        mod_ = Mode::W;
        ::lseek(file_descriptor_, w_, SEEK_SET);
    }
    auto size = ::write(file_descriptor_, buffer.data(), buffer.size());
    if (size > 0) {
        w_ += size;
    }
    return size;
}
} // namespace cpp::io