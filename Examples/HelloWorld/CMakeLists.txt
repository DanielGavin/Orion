cmake_minimum_required(VERSION 3.1)

if (WIN32 AND CMAKE_SYSTEM_VERSION)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set_property( GLOBAL PROPERTY USE_FOLDERS ON)	
else() 
    set( CMAKE_CXX_FLAGS "-std=c++11" )
endif()


include_directories(${ORION_LIB_INCLUDE_DIR})


add_executable(HelloWorld main.cpp)
target_link_libraries(HelloWorld ${SDL_LIBRARY} ${SDLMAIN_LIBRARY} Orion)