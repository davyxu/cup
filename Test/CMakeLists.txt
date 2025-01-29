



add_executable(cup_test main.cpp)


target_include_directories(cup_test PRIVATE
        ${CMAKE_SOURCE_DIR}/Foundation
        ${CMAKE_SOURCE_DIR}/Foundation/Memory
        ${CMAKE_SOURCE_DIR}/ThirdParty/spdlog-1.15.0/include
        )

target_link_libraries(cup_test Foundation)