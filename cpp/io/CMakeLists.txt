cc_library(
        NAME cpp_io
        ALIAS cpp::io
        SRCS io.cpp
        HDRS io.h
        DEPS cpp::utils
)

cc_test(
        NAME cpp_io_test
        SRCS io_test.cpp
        DEPS cpp::io
)