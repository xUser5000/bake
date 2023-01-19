# add subdirectories here
add_subdirectory(parser)
add_subdirectory(common)

add_library(bake STATIC ${ALL_OBJECT_FILES})

set(BAKE_LIBS bake_common)

target_link_libraries(bake ${BAKE_LIBS})
target_include_directories(bake PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
