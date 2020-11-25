cmake_minimum_required(VERSION 2.8.12)
project(AuthLib)

#add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

if(CONAN_LINK_RUNTIME_MULTI)
    string(REPLACE "d" "" ${CONAN_LINK_RUNTIME_MULTI} "${${CONAN_LINK_RUNTIME_MULTI}}")
    foreach(flag CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE
                 CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO
                 CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL)
        if(DEFINED ${flag})
            string(REPLACE "/MD" ${CONAN_LINK_RUNTIME_MULTI} ${flag} "${${flag}}")
        endif()
        message(STATUS "FLAG ${${flag}}: ${flag}")
    endforeach()
    foreach(flag CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG)
        if(DEFINED ${flag})
            string(REPLACE "/MDd" "${CONAN_LINK_RUNTIME_MULTI}d" ${flag} "${${flag}}")
        endif()
        message(STATUS "FLAG ${${flag}}: ${flag}")
    endforeach()
endif()

add_library(AuthLib STATIC AuthLib.cpp)
