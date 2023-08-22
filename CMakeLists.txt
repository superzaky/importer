cmake_minimum_required(VERSION 3.0.0)
project(importer VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB SOURCES "usb.cpp")
#file(GLOB SOURCES "Structure.cpp")
#file(GLOB HEADERS "Structure.hpp")

#add_library(importer STATIC ${SOURCES} ${HEADERS})

add_library(importer STATIC ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
