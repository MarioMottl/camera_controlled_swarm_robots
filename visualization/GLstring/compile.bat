mkdir obj
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/glm -I../../library/stb_master -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glc-1.0.0 -I../../library/mingw-std-threads-master -c main.cpp -o obj/main.o 
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/glm -I../../library/stb_master -c glstr_src/character_class.cpp -o obj/character_class.o 
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/glm -I../../library/stb_master -c glstr_src/font_class.cpp -o obj/font_class.o 
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/glm -I../../library/stb_master -c glstr_src/string_class.cpp -o obj/string_class.o

g++ -s -L../../library/GLFW-2.7/lib -L../../library/gl_shader-1.0.2/lib -o gl_string_test.exe obj/main.o obj/character_class.o obj/font_class.o obj/string_class.o -l_glShader ../../library/glew-2.1.0/lib/Release/Win32/glew32.lib ../../library/glew-2.1.0/lib/Release/Win32/glew32s.lib -lglfw -lopengl32

ar -s -r lib_GLstring.a obj/character_class.o obj/font_class.o obj/string_class.o
