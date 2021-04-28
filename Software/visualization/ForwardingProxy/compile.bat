mkdir obj
g++ -Wall -O3 -std=c++17 -c -Ilib/sockethandler-1.0.0/include -Ilib/cppsock -Ilib/mingw-std-threads-master main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -c lib/SchwarmPacket/packet.cpp -o obj/packet.o 
g++ -Wall -O3 -std=c++17 -c lib/SchwarmPacket/otherpacket.cpp -o obj/otherpacket.o

g++ -Llib/ws2_32 -Llib/cppsock -Llib/sockethandler-1.0.0/lib -o ForwardingProxy.exe lib/cppsock/cppsock_winonly.cpp obj/main.o obj/packet.o obj/otherpacket.o -lsockethandler -lcppsock -lws2_32 