add_library(backend FileManager.cpp)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(backend PRIVATE frontend)
