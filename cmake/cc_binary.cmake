# 参考自 https://github.com/google/ruy/tree/master/cmake

include (CMakeParseArguments)

# cc_binary()
#
# CMake function to imitate Bazel's cc_binary rule.
function(cc_binary)
    cmake_parse_arguments (
            _RULE
            ""
            "NAME"
            "SRCS;COPTS;LINKOPTS;DEPS;TAGS;INCLUDE_DIRS"
            ${ARGN}
    )

    if (DEFINED _RULE_INCLUDE_DIRS)
        # todo
    elseif (DEFINED CPP_COMMON_INCLUDE_DIRS)
        list (APPEND _RULE_INCLUDE_DIRS ${CPP_COMMON_INCLUDE_DIRS}) # 业务自定义的
    else ()
        list (APPEND _RULE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}) # 项目根路径
    endif ()

    message ("# pwd: ${CMAKE_CURRENT_LIST_DIR}
# root: ${CMAKE_SOURCE_DIR}
cc_binary(
    name: ${_RULE_NAME},
    srcs: ${_RULE_SRCS},
    deps: ${_RULE_DEPS},
    includes: ${_RULE_INCLUDE_DIRS},
    )")
    add_executable (${_RULE_NAME} "")
    target_sources (${_RULE_NAME}
            PRIVATE
            ${_RULE_SRCS}
    )
    set_target_properties (${_RULE_NAME} PROPERTIES OUTPUT_NAME "${_RULE_NAME}")
    target_include_directories (${_RULE_NAME}
            PUBLIC
            "$<BUILD_INTERFACE:${_RULE_INCLUDE_DIRS}>" # CMAKE_SOURCE_DIR
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
    target_compile_options (${_RULE_NAME}
            PRIVATE
            ${_RULE_COPTS}
    )
    target_link_options (${_RULE_NAME}
            PRIVATE
            ${_RULE_LINKOPTS}
    )
    target_link_libraries (${_RULE_NAME}
            PUBLIC
            ${_RULE_DEPS}
    )
endfunction()