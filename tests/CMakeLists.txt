add_executable(test_thread_pool test_thread_pool.cpp)
add_executable(test_parser test_parser.cpp ../src/http_parser.cpp)
add_executable(test_pipeline test_pipeline.cpp ../src/http_connection.cpp ../src/http_parser.cpp)
target_link_libraries(test_thread_pool pthread)

add_test(NAME test_thread_pool COMMAND test_thread_pool)
add_test(NAME test_parser COMMAND test_parser)
add_test(NAME test_pipeline COMMAND test_pipeline)
