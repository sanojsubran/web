project( webs )

cmake_minimum_required( VERSION 2.8 )

#TODO //Remove Hardcoded path
set( PROJECT_LOCATION "/home/xan/all_files/repos/web/" )

#add support for dependency libraries

#------------------------------------------------------------------------------
# 1. Boost
set ( BOOST_ROOT "${PROJECT_LOCATION}/dependency/Boost" )
set ( BOOST_INCLUDEDIR "${PROJECT_LOCATION}/dependency/Boost/include" )
set ( BOOST_LIBRARYDIR "${PROJECT_LOCATION}/dependency/Boost/lib" )
set ( Boost_NO_SYSTEM_PATHS "ON" )
set ( Boost_USE_STATIC_LIBS "ON" )
set ( Boost_USE_MULTITHREADED "OFF" )

find_package( Boost REQUIRED COMPONENTS system filesystem )

if( Boost_FOUND )
    message( "Boost found" )
    message( ${Boost_INCLUDE_DIRS} )
    message( ${Boost_LIBRARY_DIRS} )
else()
    message( "Boost not found" )
endif()
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
#------------------------------------------------------------------------------

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )

set( SRCS src/main.cpp                                        \\
          src/page_retriever.h                                \\
          src/page_retriever.cpp                              \\
          src/index_manager.h                                 \\
          src/index_manager.cpp                               \\
          src/context_provider.h                              \\
          src/context_provider.cpp                            #\\
          #src/search.h                                        \\
          #src/webserver.h                                     \\
          #src/webserver.cpp
          )

add_executable( ${PROJECT_NAME} ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
