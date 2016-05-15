if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
  message("Found googletest, enabling testing")
  add_subdirectory(googletest)
  
  add_executable(switch_test switch_test.cpp)
  target_link_libraries(switch_test indi_properties gtest_main indidriver)
  add_test(switch_test switch_test)
  
  add_executable(number_test number_test.cpp)
  target_link_libraries(number_test indi_properties gtest_main indidriver)
  add_test(number_test number_test)

  add_executable(properties_test properties_test.cpp)
  target_link_libraries(properties_test indi_properties gtest_main indidriver)
  add_test(properties_test properties_test)

  add_executable(properties_map_test properties_map_test.cpp)
  target_link_libraries(properties_map_test indi_properties gtest_main indidriver)
  add_test(properties_map_test properties_map_test)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
