#basic window
add_executable(basic_window basic_window/main.cpp)

target_link_libraries(basic_window voidengine)

#ui demo
add_executable(ui_demo ui_demo/main.cpp)

#input demo example
add_subdirectory(input_demo) 

target_link_libraries(ui_demo voidengine) 

