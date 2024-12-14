cmake_minimum_required(VERSION 3.4.1)

# ライブラリをビルドする
add_library(audio_processing SHARED audio_processing.c)

# include ディレクトリを指定
target_include_directories(audio_processing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
