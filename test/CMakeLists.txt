if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
  message("Found googletest, enabling testing")
  add_subdirectory(googletest)
  
  add_executable(switch_test switch_test.cpp)
  target_link_libraries(switch_test indi_properties gtest_main indidriver)
  add_test(switch_test switch_test)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
