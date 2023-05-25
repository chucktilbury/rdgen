cmake_minimum_required(VERSION 3.18)
project(rdpgen)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ./)

add_executable(${PROJECT_NAME}
    memory.c
    parser.c
    strs.c
    rdpgen.c
    dump.c
    preproc.c
    fileio.c
    scanner.c
    parser.c
    errors.c
    emit.c
    emit_ast.c
    emit_parser.c
    utils.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME}
    pthread
    gc
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        #-Wno-sign-compare
        #-Wno-unused-variable
        #-Wno-parentheses-equality
        -DUSE_GC
        $<$<CONFIG:DEBUG>:-g3 >
        $<$<CONFIG:DEBUG>:-Og >
        #$<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DENABLE_TRACE >
        $<$<CONFIG:DEBUG>:-DENABLE_DUMP >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)
