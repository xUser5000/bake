add_library(bake_common OBJECT bake.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bake_common>
        PARENT_SCOPE)
