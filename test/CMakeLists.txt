cc_test (
        NAME log_test
        SRCS log/cpp20_logger_test.cpp
        DEPS cpp::log
)

cc_test (
        NAME utils_time_test
        SRCS utils/time_test.cpp
        DEPS cpp::utils ${lib_fmt}
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

cc_test(
     NAME llhttp_test
     SRCS llhttp/llhttp_test.cpp
     DEPS ${lib_asio} ${lib_absl} ${lib_spdlog} cpp::http
)