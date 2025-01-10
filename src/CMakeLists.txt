add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(config)

add_executable(NaviDir main.cpp)

target_link_libraries(NaviDir PRIVATE frontend backend config)

target_include_directories(NaviDir PRIVATE
  ${CMAKE_SOURCE_DIR}/src/frontend
  ${CMAKE_SOURCE_DIR}/src/backend
  ${CMAKE_SOURCE_DIR}/src/config
)

target_link_libraries(NaviDir PRIVATE ftxui::dom ftxui::screen ftxui::component)

