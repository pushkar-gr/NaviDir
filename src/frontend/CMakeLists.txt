add_library(frontend UI.cpp)

target_include_directories(frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(frontend PRIVATE ftxui::screen ftxui::dom ftxui::component)
