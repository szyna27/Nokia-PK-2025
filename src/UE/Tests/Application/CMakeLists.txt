project(UeApplicationUT)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Mocks SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(SMS SRC_LIST)
aux_source_directory(States SRC_LIST)
include_directories(${COMMON_DIR}/Tests)
include_directories(${UE_DIR}/Tests)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} UeApplication)
target_link_libraries(${PROJECT_NAME} CommonUtMocks)
target_link_gtest()

add_test(NAME UeApplicationUT COMMAND ${PROJECT_NAME})




