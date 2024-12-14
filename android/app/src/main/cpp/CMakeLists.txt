cmake_minimum_required(VERSION 3.10)

project(miniaudio_wrapper)

add_library(miniaudio_wrapper SHARED
    miniaudio_wrapper.c
)

target_include_directories(miniaudio_wrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)