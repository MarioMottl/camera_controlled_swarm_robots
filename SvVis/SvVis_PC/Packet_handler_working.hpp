#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "../../library/cppsock/cppsock.hpp"
#include "../../visualization/external/SchwarmPacket/packet.h"
#include "SvVis_PC.hpp"

class SwarmCommandHandler
{
private:
	static void on_connect(std::shared_ptr<cppsock::tcp::socket> sock, cppsock::socketaddr_pair addr, void** pers)
	{
		std::cout << "[SERVER]: new client (" << addr.remote << ") connected" << std::endl;
		//*pers = new SvVis::client();
		//SVVis::client* client = (SVVis::client*)*pers;
		//client->connect();
		SvVis::client *client = new SvVis::client();
		*pers = client;
		if (client->connect(SwarmCommandHandler::conv_server) < 0)
			throw std::runtime_error("Unable to connect to conversion server");
		client->send_aq(true);
	}
	static void on_recv(std::shared_ptr<cppsock::tcp::socket> sock, cppsock::socketaddr_pair addr, void** pers)
	{
		Schwarm::VehicleCommandPacket command;
		//char buf[command.min_size()];
		std::vector<uint8_t> buf(command.min_size());
		std::stringstream ss;
		SvVis::client* client = (SvVis::client*)*pers;

		sock->recv(buf.data(), command.min_size(), cppsock::msg_waitall);
		command.allocate(command.min_size());
		command.set(buf.data());
		command.decode();

		std::cout << "[SERVER]: Command recived: " << command.get_length() << "m , " << command.get_angle() << "rad" << std::endl;
		// send length
		ss.str("");
		ss.clear();
		if (command.get_length() < 0)
			ss << "bw " << int(command.get_length() * -100);
		else if(command.get_length() > 0)
			ss << "fw " << int(command.get_length() * 100);
		std::cout << "[SERVER / SvVis]: Command to send: " << ss.str() << std::endl;
		client->send_str(ss.str());
		// send angle
		ss.str("");
		ss.clear();
		if (command.get_angle() < 0)
			ss << "rr " << int(command.get_angle() * (-180 / M_PI));
		else
			ss << "rl " << int(command.get_angle() * (+180 / M_PI));
		std::cout << "[SERVER / SvVis]: Command to send: " << ss.str() << std::endl;
		client->send_str(ss.str());

	}
	static void on_disconnect(std::shared_ptr<cppsock::tcp::socket> sock, cppsock::socketaddr_pair addr, void** pers)
	{
		SvVis::client* client = (SvVis::client*)*pers;
		std::cout << "[SERVER]: client (" << addr.remote << ") disconnected" << std::endl;
		delete client;

	}
	cppsock::tcp::socket_collection collection{ on_connect, on_recv, on_disconnect };
	cppsock::tcp::server server;
	inline static cppsock::socketaddr conv_server;
public:
	bool init(const cppsock::socketaddr& addr, const cppsock::socketaddr& conv_server)
	{
		this->conv_server = conv_server;
		this->server.set_collection(&this->collection);
		return (this->server.start(addr) >= 0);
		std::cout << "[SERVER]: started listening on " << this->server.listener().sock().getsockname() << std::endl;
	}
};
