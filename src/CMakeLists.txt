add_subdirectory(frontend)
add_subdirectory(backend)

add_executable(NaviDir main.cpp)

target_link_libraries(NaviDir PRIVATE frontend backend)
target_sources(NaviDir PRIVATE scroller/scroller.cpp)

target_include_directories(NaviDir PRIVATE
  ${CMAKE_SOURCE_DIR}/src/frontend
  ${CMAKE_SOURCE_DIR}/src/backend
)

target_link_libraries(NaviDir PRIVATE ftxui::dom ftxui::screen ftxui::component)

