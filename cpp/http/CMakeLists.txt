cc_library(
        NAME http
        ALIAS cpp::http
        HDRS common.h utils.h parser.h io.h http.h
        SRCS utils.cpp io.cpp http.cpp
        DEPS ${lib_llhttp} ${lib_fmt} ${lib_asio} ${lib_spdlog}
)