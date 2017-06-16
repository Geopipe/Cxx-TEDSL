project(CxxTEDSLtests)

add_executable(test-stringify test-stringify.cpp)
target_link_libraries(test-stringify CxxTEDSL)
