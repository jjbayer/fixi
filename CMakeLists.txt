project(fixi)
include_directories(".")

add_library(fixi
    error.cpp
    state.cpp
    token.cpp
    tokenizer.cpp
    parser.cpp
    lookup.cpp
    stack.cpp
    binop.cpp
    interpreter.cpp)

add_executable(fixi_exec fixi.cpp)
set_target_properties(fixi_exec PROPERTIES OUTPUT_NAME fixi)
target_link_libraries(fixi_exec fixi)




