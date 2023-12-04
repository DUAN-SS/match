
project(PROJECT1 CXX C)
cmake_minimum_required(VERSION 3.14)

set(PROJECT1 "PROJECT1")
# 导入Python模块
find_package(PythonLibs REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

macro(safe_set_static_flag)
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
endmacro()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/rapidjson)
include_directories(${PYTHON_INCLUDE_DIRS})

#添加所有的源文件
file(GLOB CPP_FILES     
        ${CMAKE_SOURCE_DIR}/src/*.cpp
  	)
set(PROJECT1_CPP_FILES ${CPP_FILES} "${CMAKE_SOURCE_DIR}/main.cpp")

add_executable(${PROJECT1} ${PROJECT1_CPP_FILES})

target_link_libraries(${PROJECT1} ${PYTHON_LIBRARIES})
target_link_libraries(${PROJECT1} ${DEPS})
target_compile_features(${PROJECT1} PUBLIC cxx_std_11)

if ( MSVC )
    set_target_properties( ${PROJECT1} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set_target_properties( ${PROJECT1} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
    set_target_properties( ${PROJECT1} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
endif ( MSVC )



