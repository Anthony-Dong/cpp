project(example)

file(
        GLOB_RECURSE main_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*_main.cpp"
)

foreach(main_file IN LISTS main_files)
        get_filename_component(filename ${main_file} NAME_WE)
        get_filename_component(filedir ${main_file} DIRECTORY)
        cc_binary(
                NAME "${PROJECT_NAME}_${filedir}_${filename}"
                SRCS "${main_file}"
                DEPS ${lib_spdlog} ${lib_fmt} ${lib_absl}
        )
endforeach()

file(
        GLOB_RECURSE test_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*_test.cpp"
)

foreach(test_file IN LISTS test_files)
        get_filename_component(filename ${test_file} NAME_WE)
        get_filename_component(filedir ${test_file} DIRECTORY)
        cc_test(
                NAME "${PROJECT_NAME}_${filedir}_${filename}"
                SRCS "${test_file}"
                DEPS ${lib_spdlog} ${lib_fmt} ${lib_absl}
        )
endforeach()

# asio
if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        message("build asio files")
        file(
                GLOB_RECURSE asio_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
                "${CMAKE_CURRENT_SOURCE_DIR}/*/*_asio.cpp"
        )

        foreach(asio_file IN LISTS asio_files)
                get_filename_component(filename ${asio_file} NAME_WE)
                get_filename_component(filedir ${asio_file} DIRECTORY)
                cc_binary(
                        NAME "${PROJECT_NAME}_${filedir}_${filename}"
                        SRCS "${asio_file}"
                        DEPS ${lib_asio} ${lib_spdlog} ${lib_fmt} ${lib_absl}
                )
        endforeach()

        file(
                GLOB_RECURSE asio_test_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
                "${CMAKE_CURRENT_SOURCE_DIR}/*/*_testasio.cpp"
        )

        foreach(asio_file IN LISTS asio_test_files)
                get_filename_component(filename ${asio_file} NAME_WE)
                get_filename_component(filedir ${asio_file} DIRECTORY)
                cc_test(
                        NAME "${PROJECT_NAME}_${filedir}_${filename}"
                        SRCS "${asio_file}"
                        DEPS ${lib_asio} ${lib_spdlog} ${lib_fmt} ${lib_absl}
                )
        endforeach()
endif()

# cpp20
if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        message("build cpp20 files")
        file(
                GLOB_RECURSE cpp20_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
                "${CMAKE_CURRENT_SOURCE_DIR}/*/*_cpp20.cpp"
        )

        foreach(cpp20_file IN LISTS cpp20_files)
                get_filename_component(filename ${cpp20_file} NAME_WE)
                get_filename_component(filedir ${cpp20_file} DIRECTORY)
                cc_binary(
                        NAME "${PROJECT_NAME}_${filedir}_${filename}"
                        SRCS "${cpp20_file}"
                        DEPS ${lib_spdlog} ${lib_fmt} ${lib_absl}
                )
        endforeach()
endif()

file(
        GLOB_RECURSE benchmark_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*_benchmark.cpp"
)

foreach(benchmark_file IN LISTS benchmark_files)
        get_filename_component(filename ${benchmark_file} NAME_WE)
        get_filename_component(filedir ${benchmark_file} DIRECTORY)
        cc_binary(
                NAME "${PROJECT_NAME}_${filedir}_${filename}"
                SRCS "${benchmark_file}"
                DEPS ${lib_spdlog} ${lib_fmt} ${lib_absl} ${lib_benchmark}
        )
endforeach()