#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "../../library/cppsock/cppsock.hpp"
#include "../../visualization/external/SchwarmPacket/packet.h"
#include "SvVis_PC.hpp"

class SwarmCommandHandler
{
private:
	cppsock::tcp::listener listener;
	cppsock::tcp::socket visualisation;
	cppsock::socketaddr conv_server;
	std::vector<std::shared_ptr<SvVis::client> > vehicles;
public:
	void run(const cppsock::socketaddr& addr, const cppsock::socketaddr& conv_server, size_t num_vehicles)
	{
		std::vector<uint8_t> recv_buf;
		Schwarm::VehicleCommandPacket command;
		std::stringstream ss;
		std::streamsize len;

		this->conv_server = conv_server;
		std::cout << "[SERVER]: connecting to conversion server at " << conv_server << std::endl;
		this->vehicles.resize(num_vehicles);
		for (std::shared_ptr<SvVis::client> &vehicle : vehicles)
		{
			vehicle = std::make_shared<SvVis::client>();
			if (vehicle->connect(conv_server) < 0)
			{
				std::cerr << "[SERVER]: error connecting to conversion server!" << std::endl;
				throw std::runtime_error("unable to connect to conversion server");
			}
			vehicle->send_aq(true);
		}
		this->listener.setup(addr, 1);
		std::cout << "[SERVER]: started listening on " << this->listener.sock().getsockname() << std::endl;
		this->listener.accept(this->visualisation);
		std::cout << "[SERVER]: visualisation connected" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		errno = 0;
		recv_buf.resize(command.min_size());
		while ( (len=this->visualisation.recv(recv_buf.data(), command.min_size(), cppsock::msg_waitall)) > 0)
		{
			std::cout << "data received (" << len << " bytes)" << std::endl;
			command.allocate(command.min_size());
			command.set(recv_buf.data());
			command.decode();

			std::cout << "[SERVER]: Command recived: " << command.get_length() << "m , " << command.get_angle() << "rad" << std::endl;
			// send length
			ss.str("");
			ss.clear();
			if (command.get_length() < 0)
				ss << "bw " << int(command.get_length() * -100);
			else if (command.get_length() > 0)
				ss << "fw " << int(command.get_length() * 100);
			std::cout << "[SERVER / SvVis]: Command to send: " << ss.str() << std::endl;
			this->vehicles.at(command.get_vehicle_id())->send_str(ss.str());
			// send angle
			ss.str("");
			ss.clear();
			if (command.get_angle() < 0)
				ss << "rr " << int(command.get_angle() * (-180 / M_PI));
			else
				ss << "rl " << int(command.get_angle() * (+180 / M_PI));
			std::cout << "[SERVER / SvVis]: Command to send: " << ss.str() << std::endl;
			this->vehicles.at(command.get_vehicle_id())->send_str(ss.str());
		}
		visualisation.close();
		std::cout << "recv call indicated disconnect (return code " << len << ") (errno: " << errno << ") " << strerror(errno) << std::endl;
	}
};
