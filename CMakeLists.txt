cmake_minimum_required(VERSION 3.22)
project(SeaLib C)

set(CMAKE_C_STANDARD 11)

add_library(
        Sea
        # Sources
        Sea/Person.c
        Sea/Person.h
        Sea/Random.c
        Sea/Random.h
        Sea/Uuid.c
        Sea/Uuid.h
        Sea/Arena.c
        Sea/Arena.h
        Sea/Allocator.c
        Sea/Allocator.h
)

add_executable(
        Test
        Test/Main.c
)

target_include_directories(Sea PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test Sea)
