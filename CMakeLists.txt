cmake_minimum_required(VERSION 3.10)
project(MyTask)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")


include_directories(include)

add_library(analyze STATIC src/analyze.c)

add_executable(analyze-util utils/analyze-util.c)
target_link_libraries(analyze-util analyze)

add_executable(test-util utils/test-util.c)
target_link_libraries(test-util analyze)

enable_testing()

add_test(NAME "TestAnalyzeUtil" COMMAND test-util analyze-util)

# install(TARGETS mylib myapp
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     RUNTIME DESTINATION bin
# )

# install(DIRECTORY include/
#     DESTINATION include
# )
