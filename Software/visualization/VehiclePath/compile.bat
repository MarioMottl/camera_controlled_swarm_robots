mkdir obj
g++ -Wall -O3 -std=c++17 -ID:/Michi/Programmieren/Libraries/stb_master -c main.cpp -o obj/main.o
g++ -o pathgenerator.exe obj/main.o -s