mkdir obj
g++ -Wall -O3 -std=c++17 -I../../library/cppsock -I../../library/mingw-std-threads-master -c main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -I../../library/cppsock -I../../library/mingw-std-threads-master -c msh_src/sockethandler.cpp -o obj/sockethandler.o
g++ -Wall -O3 -std=c++17 -I../../library/cppsock -I../../library/mingw-std-threads-master -c msh_src/server.cpp -o obj/server.o
g++ -Wall -O3 -std=c++17 -I../../library/cppsock -I../../library/mingw-std-threads-master -c msh_src/client.cpp -o obj/client.o

g++ -s -L../../library/cppsock -o SockenHandler.exe obj/main.o obj/sockethandler.o obj/server.o obj/client.o ../../library/cppsock/cppsock_winonly.cpp -lcppsock -lws2_32

ar -s -r libsockethandler.a obj/sockethandler.o obj/server.o obj/client.o