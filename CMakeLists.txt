cmake_minimum_required(VERSION 3.31)
project(SeaDemo C)

set(CMAKE_C_STANDARD 99)

add_executable(SeaDemo Main.sea.c
        Person.sea.c
        Person.sea.h
        Random.sea.c
        Random.sea.h
        Main.sea.h)
