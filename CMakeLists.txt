cmake_minimum_required(VERSION 3.22)
project(SEALib C)

set(CMAKE_C_STANDARD 17)

add_library(
        SEA

        SEA/Compat/AlignCompat.h
        SEA/Allocator.c
        SEA/Allocator.h
        SEA/Arena.c
        SEA/Arena.h
        SEA/Compat/ThreadCompat.h
        SEA/Error.c
        SEA/Error.h
        SEA/JSONArray.c
        SEA/JSONArray.h
        SEA/JSONObject.c
        SEA/JSONObject.h
        SEA/JSONParser.c
        SEA/JSONParser.h
        SEA/JSONType.h
        SEA/JSONValue.c
        SEA/JSONValue.h
        SEA/Utils/MacroUtils.h
        SEA/Utils/MemoryUtils.h
        SEA/Random.c
        SEA/Random.h
        SEA/Compat/StringCompat.c
        SEA/Compat/StringCompat.h
        SEA/StringBuffer.c
        SEA/StringBuffer.h
        SEA/Time.c
        SEA/Time.h
        SEA/UUID.c
        SEA/UUID.h
)

add_executable(
        Test

        Test/Main.c
        Test/Person.c
        Test/Person.h
        Test/TestJsonNumbers.c
        Test/TestJsonNumbers.h
)

if(WIN32)
    target_link_libraries(SEA bcrypt)
endif()
target_include_directories(SEA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test SEA)