project( webs )

cmake_minimum_required( VERSION 2.8 )

set( SRCS src/main.cpp              src/page_retriever.h )

add_executable( webs ${SRCS} )
