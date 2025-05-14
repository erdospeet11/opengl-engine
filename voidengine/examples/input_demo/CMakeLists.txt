set(INPUT_DEMO_NAME input_demo)

add_executable(${INPUT_DEMO_NAME} main.cpp)

target_link_libraries(${INPUT_DEMO_NAME} PRIVATE
    voidengine
)

#working directory for vs code
set_target_properties(${INPUT_DEMO_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
) 