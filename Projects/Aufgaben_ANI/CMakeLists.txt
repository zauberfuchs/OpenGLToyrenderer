cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project(Aufgaben_ANI)

include_directories("../common")


set(HEADER_FILES 	
	"../common/Aufgabe1/StudentCube.h"
	"../common/Sphere.h"
	"../common/IndexBuffer.h"
	"../common/FrameBuffer.h"
	"../common/RenderBuffer.h"
	"../common/VertexBuffer.h"
	"../common/VertexBufferLayout.h"
	"../common/ITexture.h"
	"../common/Texture.h"
	"../common/IShader.h"
	"../common/Shader.h"
	"../common/VertexArray.h"
	"../common/Skybox.h"
	"../common/Window.h"
	"../common/Plane.h"
	"../common/Scene.h"
	"../common/Camera.h"
	"../common/ICube.h"
	"../common/IMesh.h"
	"../common/IMaterial.h"
	"../common/Material.h"
	"../common/MaterialPBR.h"
	"../common/Animation.h"
	"../common/Mesh.h"
	"../common/IModel.h"
	"../common/Model.h"
	"../common/Transform.h"
	"../common/ISceneObject.h"
	"../common/SceneObject.h"
	"../common/World.h"
	)

set(DATA_SRC "../../Data")

# Find *all* shaders.
file(GLOB_RECURSE SHADERS
	"${DATA_SRC}/*.frag"
	"${DATA_SRC}/*.vert"
	)

##file(GLOB ROOT_NAT ../util/glm.natvis)

file(MAKE_DIRECTORY "../../bin/")
# execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "../../bin/")

# Separate filter for shaders.
source_group("Shaders" FILES ${SHADERS})

set(SOURCE_FILES
	"src/main.cpp"	
	"../common/Aufgabe1/StudentCube.cpp"
	"../common/Sphere.cpp"
	"../common/IndexBuffer.cpp"
	"../common/FrameBuffer.cpp"
	"../common/RenderBuffer.cpp"
	"../common/VertexBuffer.cpp"	
	"../common/Skybox.cpp"
	"../common/Plane.cpp"
	"../common/Window.cpp"
	"../common/Scene.cpp"
	"../common/Animation.cpp"
	"../common/Texture.cpp"
	"../common/Transform.cpp"
	"../common/Mesh.cpp"
	"../common/Shader.cpp"
	"../common/Material.cpp"
	"../common/MaterialPBR.cpp"
	"../common/VertexArray.cpp"
	"../common/Camera.cpp"
	"../common/Model.cpp"
	"../common/SceneObject.cpp"
	"../common/World.cpp"
	)

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
	${HEADER_FILES}
	${SOURCE_FILES}
	../common/utils/glm.natvis
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
