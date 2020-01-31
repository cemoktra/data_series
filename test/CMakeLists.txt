project(DataSeriesTest LANGUAGES CXX)

set(HEADERS
)

set(SOURCES
    main.cpp
    data_series_collection_tests.cpp
    data_series_regression_tests.cpp
    data_series_properties_tests.cpp
    functional_data_series_tests.cpp
    operational_data_series_tests.cpp
    static_data_series_tests.cpp
    mutable_data_series_tests.cpp
    resamples_data_series_tests.cpp
)

add_executable(DataSeriesTest ${SOURCES} ${HEADERS})
target_link_libraries(DataSeriesTest ${GTEST_LIBRARIES} DataSeries pthread)
gtest_add_tests(TARGET DataSeriesTest TEST_SUFFIX .noArgs TEST_LIST noArgsTests)