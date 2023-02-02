aux_source_directory(. SRC_LIST)
add_executable(MajorEleTest ${SRC_LIST})

# target_link_libraries(MajorEleTest
#     MajorEle)

add_test(NAME majoreletest COMMAND MajorEleTest)