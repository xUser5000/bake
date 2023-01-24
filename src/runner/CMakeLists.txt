add_library(bake_runner OBJECT run.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bake_runner>
        PARENT_SCOPE)
