# additional target to perform clang-format run, requires clang-format

# get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h)

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        -style=Google
        -i
        ${ALL_SOURCE_FILES}
)

