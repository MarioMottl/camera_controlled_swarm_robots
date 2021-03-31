# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project ("SchwarmGUI")

# set comiler flags
set(CMAKE_CXX_STANDARD 17)

# add external include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../library/cppsock"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/event_handler-1.3.1/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/gl_shader-1.0.2/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/gl_string-2.1.0/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/glc-1.0.0"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/glew-2.1.0/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/glfw-3.3.3/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/glm"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/model_loader-da/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../library/stb")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../library/event_handler-1.3.1/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/gl_shader-1.0.2/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/gl_string-2.1.0/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/glew-2.1.0/lib/Release/x64"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/glfw-3.3.3/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/glm/glm/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/model_loader-da/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/lib/x64")

# link event handler library
add_library(object_module STATIC 
			"${CMAKE_CURRENT_SOURCE_DIR}/client/client.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/GUI/gui_source/button.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/GUI/gui_source/gui_base.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/GUI/gui_source/gui_handler.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/GUI/gui_source/gui_renderer.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/GUI/gui_source/textbox.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/SchwarmPacket/otherpacket.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/SchwarmPacket/packet.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/Vehicle/source/vehicle_buffer_src.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/Vehicle/source/vehicle_processor_src.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/Vehicle/source/vehicle_src.cpp") 

# compile and link final executable
add_executable(SchwarmGUI 
			   "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# link external libraries to the executable
target_link_libraries(SchwarmGUI
					  "object_module"
					  "-llibevent_handler"
					  "-lgl_shader"
					  "-lgl_string"
					  "-lglm_static"
					  "-lmodel_loader"
					  "-lOpenCL"
					  "-lglfw3"
					  "-lglew32s"
					  "-lopengl32"
					  )

# additional work
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
