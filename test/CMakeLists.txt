set(test_name tfh_test)

add_executable(${test_name} ${test_name}.cpp)

target_compile_features(${test_name} PRIVATE cxx_std_17)

target_link_libraries(${test_name} PUBLIC tfh::tfh)