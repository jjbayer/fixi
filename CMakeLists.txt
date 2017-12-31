project(fixi)
include_directories("src")

add_library(fixi
    src/assign.cpp
    src/error.cpp
    src/state.cpp
    src/token.cpp
    src/tokenizer.cpp
    src/parser.cpp
    src/lookup.cpp
    src/stack.cpp
    src/binop.cpp
    src/interpreter.cpp)

add_executable(fixi_exec src/fixi.cpp)
set_target_properties(fixi_exec PROPERTIES OUTPUT_NAME fixi)
target_link_libraries(fixi_exec fixi)




