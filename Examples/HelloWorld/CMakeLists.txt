cmake_minimum_required(VERSION 3.1)

if (WIN32 AND CMAKE_SYSTEM_VERSION)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set_property( GLOBAL PROPERTY USE_FOLDERS ON)	
else() 
    set( CMAKE_CXX_FLAGS "-std=c++11" )
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${ORION_LIB_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})


add_executable(HelloWorld main.cpp GL_Renderer.cpp GL_Renderer.h)
target_link_libraries(HelloWorld ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLMAIN_LIBRARY} ${GLEW_LIBRARY} Orion)