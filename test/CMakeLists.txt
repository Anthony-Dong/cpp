cc_test (
        NAME log_test
        SRCS log/cpp20_logger_test.cpp
        DEPS cpp::log
)

cc_test (
        NAME utils_time_test
        SRCS utils/time_test.cpp
        DEPS cpp::utils fmt
)

cc_binary (
        NAME network_nio_main
        SRCS network/nio_main.cpp
        DEPS cpp::network cpp::log
)

cc_binary (
        NAME network_bio_main
        SRCS network/bio_main.cpp
        DEPS cpp::network cpp::log
)

cc_binary (
        NAME network_client_main
        SRCS network/client_main.cpp
        DEPS cpp::network cpp::log
)