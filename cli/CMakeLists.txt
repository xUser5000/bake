add_executable(cli cli.cpp)

target_link_libraries(cli bake)
set_target_properties(cli PROPERTIES OUTPUT_NAME bake)
