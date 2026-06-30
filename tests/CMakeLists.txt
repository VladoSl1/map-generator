
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(FetchContent)
    FetchContent_Declare(googletest
        URL
            https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(unit_tests
        triangleSort_test.cpp
    )
    target_link_libraries(unit_tests
        PRIVATE
            gtest_main
            map_gen_lib
    )

    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()
