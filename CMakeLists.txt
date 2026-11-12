cmake_minimum_required(VERSION 3.10.0)
project(coroutines VERSION 0.1.0 LANGUAGES C CXX)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++20)
    add_compile_options(/EHsc)
    add_compile_options(/Zc:tlsGuards)
    add_compile_options(/Zc:threadSafeInit)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall)
    add_compile_options(-std=c++20)
    add_compile_options(-fexceptions)
    add_compile_options(-O3)
    add_compile_options(-DGCC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()


add_executable(coroutines
    config.cpp
    rng.cpp
    generator.cpp
    log1.cpp
    main.cpp
)

file(COPY input DESTINATION ${CMAKE_BINARY_DIR})
