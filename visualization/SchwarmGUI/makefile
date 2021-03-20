CFLAGS = -O3 -Wall -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc"
LFLAGS = ../../library/cppsock/cppsock_winonly.cpp "../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/lib/Win32/OpenCL.lib" -l_glShader -l_GLstring ../../library/glew-2.1.0/lib/Release/Win32/glew32.lib ../../library/glew-2.1.0/lib/Release/Win32/glew32s.lib -lglfw -lopengl32 -lglu32 -lgdi32 -lModelLoader -lsockethandler -lcppsock -lws2_32 -l_event_handler -s
LIBS = -L../../library/GLFW-2.7/lib -L../../library/gl_shader-1.0.2/lib -L../../library/glstr-2.1.0/lib -L../../library/model_loader-1.0.0/lib -L../../library/ws2_32 -L../../library/cppsock -L../../library/sockethandler-1.0.0/lib -L../../library/event_handler-1.1.0/lib

CC	 = g++
EXE = ./SchwarmGUI.exe

ODIR = obj
IDIR = .

_DEPS = client/client.h \
		client/my_msh.h \
		GUI/gui_includes/button.h \
		GUI/gui_includes/textbox.h \
		GUI/gui_includes/gui_base.h \
		GUI/gui_includes/gui_events.h \
		GUI/gui_includes/gui_handler.h \
		GUI/gui_includes/gui_renderer.h \
		GUI/gui_includes/gui.h \
		SchwarmPacket/packet.h \
		Vehicle/vehicle.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = 	client/client.cpp.o \
		GUI/gui_source/button.cpp.o \
		GUI/gui_source/textbox.cpp.o \
		GUI/gui_source/gui_base.cpp.o \
		GUI/gui_source/gui_handler.cpp.o \
		GUI/gui_source/gui_renderer.cpp.o \
		SchwarmPacket/otherpacket.cpp.o \
		SchwarmPacket/packet.cpp.o \
		Vehicle/source/vehicle_buffer_src.cpp.o \
		Vehicle/source/vehicle_processor_src.cpp.o \
		Vehicle/source/vehicle_src.cpp.o \
		main.cpp.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: % $(DEPS)
	py ./makedir.py $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)

full:
	make build

build: $(OBJ)
	$(CC) $(LIBS) -o $(EXE) $(OBJ) $(LFLAGS)

run:
	$(EXE)