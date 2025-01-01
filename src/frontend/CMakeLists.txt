add_library(frontend UI.cpp)

target_link_libraries(frontend PUBLIC ftxui)
target_include_directories(frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
