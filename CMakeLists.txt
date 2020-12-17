CMAKE_MINIMUM_REQUIRED(VERSION 3.15.0)
add_compile_options("/std:c++latest")

project(winctl)

SET(target winctl)

SET(src ${CMAKE_CURRENT_SOURCE_DIR})

SET(sources ${src}/winctl/Controller.cpp)
SET(headers ${src}/winctl/Controller.hpp  ${src}/winctl/ControllerId.hpp)

#ADD_LIBRARY(${target} STATIC ${sources})
ADD_LIBRARY(${target} STATIC ${sources})

target_link_directories(${target} PUBLIC 
"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
TARGET_LINK_LIBRARIES(${target} PUBLIC dinput8 d3d10 d3dx10 dxguid)

SET_TARGET_PROPERTIES(${target} PROPERTIES PUBLIC_HEADER "${headers}")

TARGET_INCLUDE_DIRECTORIES(${target} PUBLIC ${src})

#[[
SET_TARGET_PROPERTIES(${target}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
)
TARGET_COMPILE_FEATURES(
    ${target}
    PUBLIC
        cxx_std_17
)
]]

