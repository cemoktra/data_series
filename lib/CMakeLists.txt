project(DataSeriesLib LANGUAGES CXX)

set(DATASERIERS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DATASERIERS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(HEADERS
    ${DATASERIERS_INCLUDE_DIR}/consts.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_encoding.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_collection.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_series.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_series_properties.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_series_iterator.hpp
    ${DATASERIERS_INCLUDE_DIR}/static_data_series.hpp
    ${DATASERIERS_INCLUDE_DIR}/functional_data_series.hpp
    ${DATASERIERS_INCLUDE_DIR}/operational_data_series.hpp
    ${DATASERIERS_INCLUDE_DIR}/data_series_regression.hpp
    ${DATASERIERS_INCLUDE_DIR}/mutable_data_series.hpp
)

set(SOURCES
    ${DATASERIERS_SOURCE_DIR}/data_encoding.cpp
    ${DATASERIERS_SOURCE_DIR}/data_collection.cpp
    ${DATASERIERS_SOURCE_DIR}/data_series_iterator.cpp
    ${DATASERIERS_SOURCE_DIR}/static_data_series.cpp
    ${DATASERIERS_SOURCE_DIR}/functional_data_series.cpp
    ${DATASERIERS_SOURCE_DIR}/operational_data_series.cpp
    ${DATASERIERS_SOURCE_DIR}/data_series_regression.cpp
    ${DATASERIERS_SOURCE_DIR}/mutable_data_series.cpp
)

include_directories(${DATASERIERS_INCLUDE_DIR})

add_library(DataSeries ${SOURCES} ${HEADERS})
target_include_directories(DataSeries INTERFACE
    $<BUILD_INTERFACE:${DATASERIERS_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(DataSeries xtl xtensor xtensor-blas lapack openblas OpenSSL::Crypto ZLIB::ZLIB)