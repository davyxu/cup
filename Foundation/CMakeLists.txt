

list(APPEND foundation_SRC
        Application.cpp
        Memory/GarbageCollector.cpp
        Memory/MemoryPool.cpp
        Memory/ImmutableString.cpp
        )


add_library(Foundation STATIC ${foundation_SRC})

target_include_directories(Foundation PRIVATE
        ${CMAKE_SOURCE_DIR}/ThirdParty/spdlog-1.15.0/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory
        #${CMAKE_CURRENT_SOURCE_DIR}
        )