cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project(Aufgaben_ANI)

include_directories("../common")

set(DATA_SRC "../../Data")
set(CORE_SRC "../../Engine")

file(GLOB_RECURSE UTIL_FILES
	"${CORE_SRC}/Utils/*.h"
	)

file(GLOB_RECURSE INTERFACE_FILES
	"${CORE_SRC}/Interfaces/*.h"
	)

file(GLOB_RECURSE OPENGL_FILES
	"${CORE_SRC}/OpenGL/*.h"
	"${CORE_SRC}/OpenGL/*.cpp"
	)
file(GLOB_RECURSE SCENE_FILES
	"${CORE_SRC}/Scene/*.h"
	"${CORE_SRC}/Scene/*.cpp"
	)
file(GLOB_RECURSE COMPONENT_FILES
	"${CORE_SRC}/Components/*.h"
	"${CORE_SRC}/Components/*.cpp"
	)

file(GLOB_RECURSE WINDOW_FILES
	"${CORE_SRC}/Window/*.h"
	"${CORE_SRC}/Window/*.cpp"
	)

set(APPLICATION
 	"src/main.cpp"	
 	)

# Find *all* shaders.
file(GLOB_RECURSE SHADERS
	"${DATA_SRC}/*.frag"
	"${DATA_SRC}/*.vert"
	)
	
file(MAKE_DIRECTORY "../../bin/")
	# execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "../../bin/")
	
# Separate filters.
source_group("Shaders" FILES ${SHADERS})
source_group("Interfaces" FILES ${INTERFACE_FILES})
source_group("OpenGL" FILES ${OPENGL_FILES})
source_group("Scene" FILES ${SCENE_FILES})
source_group("Components" FILES ${COMPONENT_FILES})
source_group("Window" FILES ${WINDOW_FILES})
source_group("Util" FILES ${UTIL_FILES})

set(OPEN_GL_FILES 	
	glu32
	opengl32
	)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# Build and link executable.
add_executable(
	${PROJECT_NAME}
	${SHADERS}
	${INTERFACE_FILES}
	${APPLICATION}
	${WINDOW_FILES}
	${SCENE_FILES}
	${OPENGL_FILES}
	${COMPONENT_FILES}
	${UTIL_FILES}
	../../Engine/utils/glm.natvis
	)

if(WIN32)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE glm::glm
		PRIVATE GLEW::GLEW
		PRIVATE glfw
		PRIVATE imgui::imgui
		${OPEN_GL_FILES}
		PRIVATE assimp::assimp
		)
		
else()
	target_link_libraries(${PROJECT_NAME}
		PRIVATE glm::glm
		PRIVATE GLEW::GLEW
		PRIVATE glfw
		PRIVATE imgui::imgui
		PRIVATE assimp::assimp
		GL
		GLU		
		)
	
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/glew32.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/glfw3.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/assimp-vc142-mt.dll"		# <--this is in-file
		"../../../bin")														# <--this is out-file path

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/zlib1.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path

		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD				# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/irrlicht.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/jpeg62.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/libpng16.dll"				# <--this is in-file
		"../../../bin")														# <--this is out-file path


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD						# Adds a post-build event to MyTest
	COMMAND ${CMAKE_COMMAND} -E copy_if_different							# which executes "cmake - E copy_if_different..."
		"../../../vcpkg/installed/x64-windows/bin/bz2.dll"					# <--this is in-file
		"../../../bin")														# <--this is out-file path


## set working directory to bin directory
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "../../../bin")

## set the startup project to the current project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
