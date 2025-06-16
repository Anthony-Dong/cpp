cc_library(
        NAME cpp_log
        ALIAS cpp::log cpp::utils
        HDRS logger.h
        DEPS ${lib_fmt} ${lib_absl}
)