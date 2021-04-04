import sys
import serial
import socket
import threading
import argparse

_global_ip = "0.0.0.0"
_global_port = 0
_global_comports = []

def getArgs():
    global _global_ip
    global _global_port
    global _global_comports
    baudrate = 9600

    parser = argparse.ArgumentParser()
    parser.add_argument('--port', help = "Port")
    parser.add_argument('--ip', help = "IP - Address")
    parser.add_argument('--comports',help = "Com-Ports")
    parser.add_argument('--baudrate', help= "COM BAudrate")
    args = parser.parse_args()
    if args.port:
        port = args.port
        _global_port = int(port)
        print(port)
    else:
        print("[tcptocom] No TCP Port specified")
        exit(-1)
    if args.ip:
        ip = args.ip
        _global_ip = ip
        print(ip)
    if args.baudrate:
        baudrate = args.baudrate;
    if args.comports:
        comPorts = args.comports.split(",");
        print (comPorts)
        for comPort in comPorts:
            _global_comports.append(open_com(comPort, baudrate))

def open_com(comport, baudrate):
    print("[tcptocom] opening COM port %s with baudrate %d" % (comport, baudrate))
    try:
        serielle = serial.Serial(comport, baudrate, timeout = 0)
    except serial.SerialException as exception:
        print("========== Opening Serial Port failed ==========")
        print(type(exception))
        print(exception.args[0])
        exit(-1)
        pass
    print("[tcptocom] comport %s opened with baud rate %d" % (comport, baudrate) )
    return serielle

def tcptoserial(tcp,serielle):
    running = true
    while running:
        data = tcp.recv(1024)
        if data:
            print("[tcptocom] Data from tcp to serial = ",data)
            serielle.write(data);
        else:
            # tcp socket was closed, shutting program down
            running = False
    tcp.shutdown()

def serialtotcp(tcp,serielle):
    running = true
    while running:
        data = serielle.readline(1024)
        if data:
            print("[tcptocom] Data from serial to tcp = ",data)
            tcp.send(data)

def match_connection(sock):
    global _global_comports

    print("[tcptocom] connecting client with comport")
    serial = _global_comports.pop(0)
    tcptos = threading.Thread(target = tcptoserial, args = (sock, serial))
    stotcp = threading.Thread(target = serialtotcp, args = (sock, serial))

    tcptos.join();
    stotcp.join();

def main():
    global _global_comports
    global _global_ip
    global _global_port
    _matcher_threads = []

    getArgs()
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    listener.bind((_global_ip, _global_port))
    listener.listen()
    print("[tcptocom] TCP server listening at %s %d" % (_global_ip, _global_port) )
    while (len(_global_comports) > 0):
        con, addr = listener.accept()
        print("[tcptocom] client connected: ", addr)
        _thread = threading.Thread(target=match_connection, args = (con) )
        print("[tcptocom] matching thread started")
        _matcher_threads.append( _thread )

    print("[tcptocom] closing server and program...")
    listener.close();

    for _thread in _matcher_threads:
        _thread.join();

main()