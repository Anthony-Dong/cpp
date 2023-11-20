project (example)

file (
        GLOB_RECURSE main_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*_main.cpp"
)

foreach (main_file IN LISTS main_files)
    get_filename_component (filename ${main_file} NAME_WE)
    get_filename_component (filedir ${main_file} DIRECTORY)
    cc_binary (
            NAME "${PROJECT_NAME}_${filedir}_${filename}"
            SRCS "${main_file}"
            DEPS ${THIRD_PART_LIB}
    )
endforeach ()