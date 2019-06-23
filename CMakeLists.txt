cmake_minimum_required(VERSION 3.6)
project(c8)

add_subdirectory(${c8_SOURCE_DIR}/ext/rxi/)
include_directories(${c8_SOURCE_DIR}/ext/)
link_directories(${c8_SOURCE_DIR}/ext/)

include_directories(${c8_SOURCE_DIR})
link_directories(${c8_SOURCE_DIR})

add_subdirectory(${c8_SOURCE_DIR}/c8)
include_directories(${c8_SOURCE_DIR}/c8)
link_directories(${c8_SOURCE_DIR}/c8)

add_subdirectory(${c8_SOURCE_DIR}/c8-disassembler/)
include_directories(${c8_SOURCE_DIR}/c8-disassembler/)
