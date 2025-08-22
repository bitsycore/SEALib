cmake_minimum_required(VERSION 3.22)
project(SEALib C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(BlockRuntime)

add_library(
        SEA

        SEA/Allocator.c
        SEA/Allocator.h
        SEA/Arena.c
        SEA/Arena.h
        SEA/Compat/AlignCompat.h
        SEA/Compat/MathCompat.c
        SEA/Compat/MathCompat.h
        SEA/Compat/StringCompat.c
        SEA/Compat/StringCompat.h
        SEA/Compat/ThreadCompat.h
        SEA/Compat/TypeCompat.h
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
        SEA/ListChunked.c
        SEA/ListChunked.h
        SEA/ListDyn.c
        SEA/ListDyn.h
        SEA/ListSegmented.c
        SEA/ListSegmented.h
        SEA/Random.c
        SEA/Random.h
        SEA/StringBuffer.c
        SEA/StringBuffer.h
        SEA/Time.c
        SEA/Time.h
        SEA/Utils/ScopeUtils.h
        SEA/UUID.c
        SEA/UUID.h
        SEA/Iterator.c
        SEA/Iterator.h
)

add_executable(
        Test

        Test/Main.c
        Test/Person.c
        Test/Person.h
        Test/TestArena.c
        Test/TestArena.h
        Test/TestJsonBatch.c
        Test/TestJsonBatch.h
        Test/TestJsonGeneric.c
        Test/TestJsonGeneric.h
        Test/TestJsonNumbers.c
        Test/TestJsonNumbers.h
        Test/TestLists.c
        Test/TestLists.h
)

add_executable(
        TestBlock

        TestBlock/main.c
        TestBlock/Lazy.h
        TestBlock/Macro.h
)


if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(TestBlock PRIVATE -fblocks)
    target_link_libraries(TestBlock PRIVATE BlockRuntime)
    add_custom_command(TARGET TestBlock POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:BlockRuntime>
            $<TARGET_FILE_DIR:TestBlock>
    )
endif ()

if(WIN32)
    target_link_libraries(SEA bcrypt)
endif()

target_include_directories(SEA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Test PRIVATE SEA BlockRuntime)
