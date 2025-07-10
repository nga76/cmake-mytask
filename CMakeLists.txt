cmake_minimum_required(VERSION 3.10)
project(MyTask)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")


include_directories(include)

add_library(analyze STATIC src/analyze.c)

add_executable(analyze-util utils/analyze-util.c)
target_link_libraries(analyze-util analyze)

enable_testing()
add_executable(tests tests/test_analyze.c)
target_link_libraries(tests analyze)

add_test(NAME TestAnalyzeLib COMMAND tests)

# install(TARGETS mylib myapp
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     RUNTIME DESTINATION bin
# )

# install(DIRECTORY include/
#     DESTINATION include
# )
