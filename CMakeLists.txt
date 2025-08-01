cmake_minimum_required(VERSION 3.22)
project(SEALib C)

set(CMAKE_C_STANDARD 11)

add_library(
        SEA

        SEA/Random.c
        SEA/Random.h
        SEA/UUID.c
        SEA/UUID.h
        SEA/Arena.c
        SEA/Arena.h
        SEA/Allocator.c
        SEA/Allocator.h
        SEA/Compat.h
        SEA/Error.c
        SEA/Error.h
        SEA/Memory.h
        SEA/Time.c
        SEA/Time.h
        SEA/StringBuffer.c
        SEA/StringBuffer.h
        SEA/Macro.h
        SEA/Align.h

        SEA/JSON/JSONValue.c
        SEA/JSON/JSONValue.h
        Sea/JSON/JSONArray.c
        SEA/JSON/JSONArray.h
        SEA/JSON/JSONObject.c
        SEA/JSON/JSONObject.h
        SEA/JSON/JSONParser.c
        SEA/JSON/JSONParser.h

)

add_executable(
        Test
        Test/Main.c
        Test/Person.c
        Test/Person.h
)

if(WIN32)
    target_link_libraries(SEA bcrypt)
endif()
target_include_directories(SEA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test SEA)