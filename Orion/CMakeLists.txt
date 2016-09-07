cmake_minimum_required(VERSION 3.1)


if (WIN32 AND CMAKE_SYSTEM_VERSION)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set_property( GLOBAL PROPERTY USE_FOLDERS ON)	
else() 
    set( CMAKE_CXX_FLAGS "-std=c++11" )
endif()


FIND_PACKAGE( SDL REQUIRED)


set(ORION_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
				           ${SDL_INCLUDE_DIR}			
)

set(ORION_LIB_INCLUDE_DIR ${ORION_LIB_INCLUDE_DIR}  PARENT_SCOPE)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADER
	${SRC_DIR}/Gui.h
	${SRC_DIR}/CommandQueue.h
	${SRC_DIR}/Layout.h
	${SRC_DIR}/Style.h
	${SRC_DIR}/Color.h
	${SRC_DIR}/Math.h
)

set(SOURCE 
	${SRC_DIR}/Gui.cpp
	${SRC_DIR}/Layout.cpp
	${SRC_DIR}/Style.cpp
	${SRC_DIR}/Color.cpp
	${SRC_DIR}/Math.cpp
)

SOURCE_GROUP_BY_FOLDER("${CMAKE_CURRENT_SOURCE_DIR}/src" ${HEADER} ${SOURCE})

if(TARGET Orion)
	message("Orion is already defined.")
else()
	include_directories(${ORION_LIB_INCLUDE_DIR})
	add_library(Orion STATIC ${SOURCE} ${HEADER})
endif()