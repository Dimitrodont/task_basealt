cmake_minimum_required(VERSION 2.6)	# min version 

project(test)				# project name
					# ${PROJECT_NAME} - get project name
set(CMAKE_CXX_STANDART 11)		# set C++ standart

#*********CURL*********
find_library(CURL_LIB curl)

if(${CURL_LIB})
	message(FATAL_ERROR "no curl")
else()
	message(STATUS "curl found! as ${CURL_LIB}")
endif()

#*********CREATE A LIB*********
add_library(
	_data_getter SHARED
	data_getter.hpp
	data_getter.cpp
)
target_link_libraries(_data_getter PRIVATE ${CURL_LIB})	# link curl to my lib

#*********CREATE AN EXE*********
add_executable(${PROJECT_NAME} main.cpp)	# create exe file

#*********LINK MY LIB*********
target_link_libraries(${PROJECT_NAME} PRIVATE _data_getter)	# linking
