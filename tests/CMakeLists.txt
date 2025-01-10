find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
  test_main.cpp
  test_UI.cpp
  test_FileManager.cpp
)

add_executable(NaviDirTests ${TEST_SOURCES})

target_link_libraries(NaviDirTests PRIVATE frontend backend config GTest::GTest GTest::Main)
target_link_libraries(NaviDirTests PRIVATE ftxui::dom ftxui::screen ftxui::component)

target_include_directories(NaviDirTests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/frontend
  ${CMAKE_SOURCE_DIR}/src/backend
)

enable_testing()

add_test(NAME NaviDirTests COMMAND NaviDirTests)
