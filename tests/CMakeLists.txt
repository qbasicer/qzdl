CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(test_zdl LANGUAGES CXX)


find_package(Qt5Test REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 11)
ENABLE_TESTING()

add_executable(
	${PROJECT_NAME}
	tst_qzdl.cpp
	../src/libwad.cpp
)
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Test Qt5::Core)
target_include_directories(${PROJECT_NAME} PRIVATE ../include)

file(COPY resources/dummy.wad DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
