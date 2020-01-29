cmake_minimum_required(VERSION 3.12)

project(DataSeries LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(GoogleTest)

message("CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")
message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(submodules/xtl EXCLUDE_FROM_ALL )
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/submodules/xtl")
add_subdirectory(submodules/xtensor EXCLUDE_FROM_ALL )
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/submodules/xtensor")
add_subdirectory(submodules/xtensor-blas EXCLUDE_FROM_ALL )

include_directories(${xtl_INCLUDE_DIRS})
include_directories(${xtensor_INCLUDE_DIRS})
include_directories(${xtensor-blas_INCLUDE_DIRS})

add_subdirectory(lib)
add_subdirectory(test)