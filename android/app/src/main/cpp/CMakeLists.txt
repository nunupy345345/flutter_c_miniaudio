cmake_minimum_required(VERSION 3.4.1)

add_library(audio_processing SHARED audio_processing.c)

target_include_directories(audio_processing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
