mkdir obj
g++ -Wall -O3 -std=c++17 -ID:/Michi/Programmieren/Libraries/cppsock -ID:/Michi/Programmieren/Libraries/sockethandler-1.0.0/include -ID:/Michi/Programmieren/Libraries/mingw-std-threads-master -c main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -ID:/Michi/Programmieren/Libraries/cppsock -ID:/Michi/Programmieren/Libraries/sockethandler-1.0.0/include -ID:/Michi/Programmieren/Libraries/mingw-std-threads-master -c SchwarmPacket/packet.cpp -o obj/packet.o
g++ -Wall -O3 -std=c++17 -ID:/Michi/Programmieren/Libraries/cppsock -ID:/Michi/Programmieren/Libraries/sockethandler-1.0.0/include -ID:/Michi/Programmieren/Libraries/mingw-std-threads-master -c SchwarmPacket/otherpacket.cpp -o obj/otherpacket.o
g++ -LC:/CodeBlocks/gcc-8.2-32/i686-pc-mingw32/lib -LD:/Michi/Programmieren/Libraries/sockethandler-1.0.0/lib -LD:/Michi/Programmieren/Libraries/cppsock -o path_server.exe D:/Michi/Programmieren/Libraries/cppsock/cppsock_winonly.cpp obj/main.o obj/packet.o obj/otherpacket.o -lsockethandler -lcppsock -lws2_32 -s
