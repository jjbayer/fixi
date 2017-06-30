include_directories(".")

add_library(jopl
    state.cpp
    token.cpp
    tokenizer.cpp
    parser.cpp
    interpreter.cpp)

add_executable(jopl_exec jopl.cpp)
set_target_properties(jopl_exec PROPERTIES OUTPUT_NAME jopl)
target_link_libraries(jopl_exec jopl)




