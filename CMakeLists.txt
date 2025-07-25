cmake_minimum_required(VERSION 3.31)
project(SeaDemo C)

set(CMAKE_C_STANDARD 99)

add_executable(
        SeaDemo
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
)