# add subdirectories here

set(BAKE_LIBS)

add_library(bake STATIC ${ALL_OBJECT_FILES})
target_link_libraries(bake ${BAKE_LIBS})
target_include_directories(bake PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
