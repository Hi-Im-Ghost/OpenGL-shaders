# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)

set(GLUT_INCLUDE_DIR external/freeglut/include)
set(GLUT_LIB_DIR external/freeglut/lib/x64)

include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIB_DIR})


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	external/SOIL2-master/incs
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	freeglut
)
set(ALL_LIBS_GLUT
		${OPENGL_LIBRARY}
		GLEW_1130
		freeglut
		)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Lab1
add_executable(lab1
	lab1/lab1.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp

	lab1/TextureFragmentShader.fragmentshader
	lab1/TransformVertexShader.vertexshader
)
target_link_libraries(lab1
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab1 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lab1/")
create_target_launcher(lab1 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lab1/")

# Lab2
add_executable(lab2
	lab2/lab2.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/stb_image.c
	common/stb_image.h

	lab2/TextureFragmentShader.fragmentshader
	lab2/TransformVertexShader.vertexshader
		lab2/Shader.cpp lab2/Shader.h lab2/Texture.cpp lab2/Texture.h lab2/Mesh.cpp lab2/Mesh.h lab2/Primitives/Primitives.cpp lab2/Primitives/Primitives.h lab2/Primitives/Vertex.h)
target_link_libraries(lab2
	${ALL_LIBS}

)
# Xcode and Visual working directories
set_target_properties(lab2 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lab2/")
create_target_launcher(lab2 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lab2/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET lab1 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab1${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/lab1/"
)
add_custom_command(
   TARGET lab2 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab2${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/lab2/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

