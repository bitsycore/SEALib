cmake_minimum_required(VERSION 3.22)
project(SeaLib C)

set(CMAKE_C_STANDARD 23)

add_executable(
        SeaLib
        # Sources
        Main.c
        Person.c
        Person.h
        Random.c
        Random.h
        Uuid.c
        Uuid.h
        Arena.c
        Arena.h
        Allocator.c
        Allocator.h
)