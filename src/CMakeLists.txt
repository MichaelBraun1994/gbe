set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(gbe main.cpp)
target_link_libraries(gbe
    PRIVATE
    SDL3::SDL3
    imgui
)