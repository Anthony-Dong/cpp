#pragma once

#include <string>
#include <mutex>
#include "cpp/utils/class.h"

namespace cpp::io {

struct ReadWriter {
    virtual ~ReadWriter() = default;

    virtual size_t read(std::string &buffer) = 0;

    virtual size_t write(const std::string &buffer) = 0;
};

struct FileReadWriter : ReadWriter {
    explicit FileReadWriter(std::string filename);

    ~FileReadWriter() override;

    DeleteCopyConstructor(FileReadWriter);

    size_t read(std::string &buffer) override;

    size_t write(const std::string &buffer) override;

private:
    enum Mode {
        W,
        R,
    };

private:
    std::mutex mutex_;
    Mode mod_;
    // w r offset
    off_t r_;
    off_t w_;
    int file_descriptor_;
    std::string filename_;
};
} // namespace cpp::io
