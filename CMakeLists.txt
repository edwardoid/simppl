#
# Setup the cmake file to be added with a find_package
# or an add_subdirectory.
#
cmake_minimum_required(VERSION 3.5)
project(simppl VERSION 0.3.0)

# Configuration specific to being the main project
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(SIMPPL_NOT_SUBPROJECT ON)

    include(CTest)
endif()

# Reference the FindDBus module until an upstream one is available
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# enable introspection by default
option(HAVE_INTROSPECTION "Have introspection" ON)
include(CMakeDependentOption)
cmake_dependent_option(SIMPPL_BUILD_TESTS "Build tests"
    ON "SIMPPL_NOT_SUBPROJECT" OFF
)
cmake_dependent_option(SIMPPL_BUILD_EXAMPLES "Build examples"
    ON "SIMPPL_NOT_SUBPROJECT" OFF
)

# enable threading
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(DBus REQUIRED)
find_package(Boost)

add_library(${PROJECT_NAME} SHARED
    src/callstate.cpp
    src/dispatcher.cpp
    src/error.cpp
    src/serverresponseholder.cpp
    src/skeletonbase.cpp
    src/serverside.cpp
    src/stubbase.cpp
    src/timeout.cpp
    src/util.cpp
    src/serverrequestdescriptor.cpp
    src/pendingcall.cpp
    src/string.cpp
    src/wstring.cpp
    src/objectpath.cpp
    src/filedescriptor.cpp
    src/clientside.cpp
    src/serialization.cpp
    src/bool.cpp
)
# Provide a namespaced alias
add_library(Simppl::simppl ALIAS ${PROJECT_NAME})

if(HAVE_INTROSPECTION)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SIMPPL_HAVE_INTROSPECTION=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC SIMPPL_HAVE_INTROSPECTION=0)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall)
target_link_libraries(${PROJECT_NAME} PUBLIC
    DBUS::DBUS
    ${CMAKE_THREAD_LIBS_INIT}
)

if (SIMPPL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SIMPPL_BUILD_TESTS AND BUILD_TESTING)
    add_subdirectory(tests)
endif()


#
# Installation of stuff
#
if(SIMPPL_NOT_SUBPROJECT)
    include(GNUInstallDirs)

    install(TARGETS ${PROJECT_NAME} EXPORT simpplConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(EXPORT ${PROJECT_NAME}Config DESTINATION /usr/share/simppl/cmake)

    export(TARGETS ${PROJECT_NAME} FILE simpplConfig.cmake)
endif()
