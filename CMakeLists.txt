project( webs )

cmake_minimum_required( VERSION 2.8 )

#TODO //Remove Hardcoded path
set( PROJECT_LOCATION "E:/personal/code/_web/web" )

#add support for dependency libraries

#------------------------------------------------------------------------------
# 1. Boost
set ( BOOST_ROOT "${PROJECT_LOCATION}/dependency/Boost" )
set ( BOOST_INCLUDEDIR "${PROJECT_LOCATION}/dependency/Boost/include" )
set ( BOOST_LIBRARYDIR "${PROJECT_LOCATION}/dependency/Boost/lib" )
set ( Boost_NO_SYSTEM_PATHS "ON" )
set ( Boost_USE_STATIC_LIBS "ON" )
set ( Boost_USE_MULTITHREADED "ON" )

find_package( Boost )
if( Boost_FOUND )
    message( "Boost found" )
else()
    message( "Boost not found" )
endif()
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES})
#------------------------------------------------------------------------------


set( SRCS src/main.cpp             \\
          src/search.h             \\
          src/page_retriever.h )

add_executable( ${PROJECT_NAME} ${SRCS} )
