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
        Sea/Compat.h
        Sea/Error.c
        Sea/Error.h
        Sea/Memory.h
        Sea/Time.c
        Sea/Time.h
        SEA/JSON/JSONValue.c
        SEA/JSON/JSONValue.h
        SEA/StringBuffer.c
        SEA/StringBuffer.h
        Sea/Json/JsonArray.c
        Sea/Json/JsonArray.h
        Sea/Json/JSONObject.c
        Sea/Json/JSONObject.h
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
    target_link_libraries(Sea bcrypt)
endif()
target_include_directories(Sea PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test Sea)
