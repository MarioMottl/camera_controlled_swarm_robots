mkdir obj
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/glm -I../../library/GLFW-2.7/include -c main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -c Shader_src/shader_load_src.cpp -o obj/shader_load_src.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -c Shader_src/shader_other_src.cpp -o obj/shader_other_src.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -c Shader_src/shader_uniform_src.cpp -o obj/shader_uniform_src.o

g++ -s -L../../library/GLFW-2.7/lib -o shader_test.exe obj/main.o obj/shader_load_src.o obj/shader_other_src.o obj/shader_uniform_src.o ../../library/glew-2.1.0/lib/Release/Win32/glew32.lib ../../library/glew-2.1.0/lib/Release/Win32/glew32s.lib -lglfw -lopengl32

ar -s -r lib_glShader.a obj/shader_load_src.o obj/shader_other_src.o obj/shader_uniform_src.o