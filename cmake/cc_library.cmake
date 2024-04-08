# 参考自 https://github.com/google/ruy/tree/master/cmake

include (CMakeParseArguments)

# cc_library()
#
# CMake function to imitate Bazel's cc_library rule.
function(cc_library)
    cmake_parse_arguments (
            _RULE
            "TESTONLY"
            "NAME;ALIAS"
            "HDRS;SRCS;COPTS;DEFINES;LINKOPTS;DEPS;INCLUDE_DIRS"
            ${ARGN}
    )

    # Check if this is a header-only library.
    if ("${_RULE_SRCS}" STREQUAL "")
        set (_RULE_IS_INTERFACE 1)
    else ()
        set (_RULE_IS_INTERFACE 0)
    endif ()

    if (DEFINED _RULE_INCLUDE_DIRS)
        # todo
    elseif (DEFINED CPP_COMMON_INCLUDE_DIRS)
        list (APPEND _RULE_INCLUDE_DIRS ${CPP_COMMON_INCLUDE_DIRS}) # 业务自定义的
    else ()
        list (APPEND _RULE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}) # 项目根路径
    endif ()

    file (RELATIVE_PATH _RULE_SUBDIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR})

    message ("# pwd: ${CMAKE_CURRENT_LIST_DIR}
# root: ${CMAKE_SOURCE_DIR}
cc_library(
    name: ${_RULE_NAME},
    alias: ${_RULE_ALIAS},
    srcs: ${_RULE_SRCS},
    hdrs: ${_RULE_HDRS},
    deps: ${_RULE_DEPS},
    includes: ${_RULE_INCLUDE_DIRS},
    )")

    if (_RULE_IS_INTERFACE)
        # Generating a header-only library.
        add_library (${_RULE_NAME} INTERFACE)
        set_target_properties (${_RULE_NAME} PROPERTIES PUBLIC_HEADER "${_RULE_HDRS}")
        target_include_directories (${_RULE_NAME}
                INTERFACE
                "$<BUILD_INTERFACE:${_RULE_INCLUDE_DIRS}>" # 指定include路径
                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>" # 指定install路径
        )
        target_link_libraries (${_RULE_NAME}
                INTERFACE
                ${_RULE_DEPS}
                ${_RULE_LINKOPTS}
        )
        target_compile_definitions (${_RULE_NAME}
                INTERFACE
                ${_RULE_DEFINES}
        )
    else ()
        # Generating a static binary library.
        add_library (${_RULE_NAME} STATIC ${_RULE_SRCS} ${_RULE_HDRS})
        set_target_properties (${_RULE_NAME} PROPERTIES PUBLIC_HEADER "${_RULE_HDRS}")
        target_include_directories (${_RULE_NAME}
                PUBLIC
                "$<BUILD_INTERFACE:${CPP_COMMON_INCLUDE_DIRS}>" # CMAKE_SOURCE_DIR/PROJECT_SOURCE_DIR
                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        )
        target_compile_options (${_RULE_NAME}
                PRIVATE
                ${_RULE_COPTS}
        )
        target_link_libraries (${_RULE_NAME}
                PUBLIC
                ${_RULE_DEPS}
                PRIVATE
                ${_RULE_LINKOPTS}
        )
        target_compile_definitions (${_RULE_NAME}
                PUBLIC
                ${_RULE_DEFINES}
        )
    endif ()

    if (NOT _RULE_TESTONLY)
        install (
                TARGETS ${_RULE_NAME}
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${_RULE_SUBDIR}
        )
    endif ()
    if (NOT "${_RULE_ALIAS}" STREQUAL "")
        add_library (${_RULE_ALIAS} ALIAS ${_RULE_NAME})
    endif ()
endfunction()