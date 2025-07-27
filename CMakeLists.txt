cmake_minimum_required(VERSION 3.22)
project(SeaLib C)

set(CMAKE_C_STANDARD 11)

add_library(
        Sea
        # Sources
        Sea/Random.c
        Sea/Random.h
        Sea/UUID.c
        Sea/UUID.h
        Sea/Arena.c
        Sea/Arena.h
        Sea/Allocator.c
        Sea/Allocator.h
        Sea/ThreadLocal.h
        Sea/Error.c
        Sea/Error.h
)

add_executable(
        Test
        Test/Main.c
        Test/Person.c
        Test/Person.h
)

if(WIN32)
    target_link_libraries(Sea bcrypt)
endif()
target_include_directories(Sea PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test Sea)
