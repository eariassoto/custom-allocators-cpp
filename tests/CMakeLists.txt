set(UNIT_TEST_LIST
  stack_allocator)

foreach(NAME IN LISTS UNIT_TEST_LIST)
  list(APPEND UNIT_TEST_SOURCE_LIST ${NAME}.test.cpp)
endforeach()

set(TARGET_NAME tests)

add_executable(${TARGET_NAME}
  main.cpp
  ${UNIT_TEST_SOURCE_LIST})

# Replace this name with your main project name
target_link_libraries(${TARGET_NAME}
  PUBLIC custom-allocators)

target_include_directories(${TARGET_NAME}
  PUBLIC ../external/catch2/)

add_test(
  NAME ${TARGET_NAME}
  COMMAND ${TARGET_NAME} -o report.xml -r junit)
