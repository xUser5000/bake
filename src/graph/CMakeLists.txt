add_library(bake_graph OBJECT Graph.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bake_graph>
        PARENT_SCOPE)
