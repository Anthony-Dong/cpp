cc_library(
        NAME cpp_network
        ALIAS cpp::network
        SRCS listener.cpp
        HDRS header.h utils.h task_queue.h listener.h
        DEPS ${lib_libevent} cpp::utils
)

cc_test(
        NAME cpp_network_test
        SRCS network_test.cpp
        DEPS cpp::network
)