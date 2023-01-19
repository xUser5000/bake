add_library(bake_parser OBJECT Rule.cpp Token.cpp Tokenizer.cpp Parser.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:bake_parser>
        PARENT_SCOPE)
