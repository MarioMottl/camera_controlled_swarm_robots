#include "SvVis_PC.hpp"
#include "Packet_handler.hpp"

#if 1

//#define CONTINUUS

int main()
{
#ifdef CONTINUUS
	for (;;)
	{
#endif
		SwarmCommandHandler handler;
		//if(std::system("python ../../../tcptocom.py --port 10010 --comports COM6") == 0)
		std::thread python(std::system, "python.exe \"../../../tcptocom (old).py\" 0.0.0.0 10010 COM11 9600");
		handler.run(cppsock::make_any<cppsock::IPv4>(10002), cppsock::make_loopback<cppsock::IPv4>(10010), 1);
		python.join();
#ifdef CONTINUUS
	}
#endif
	return 0;
}

#else
void print_answers(SvVis::client* con, std::atomic_bool *running)
{
	SvVis::message_t msg;
	while (*running)
	{
		*running = con->recv_msg(msg);
		std::cout << "[recv]: " << msg.data.raw << std::endl;
	}
}

int main()
{
	std::atomic_bool running = true;
	SvVis::client car_con;
	SvVis::message_t msg;
	std::cout << "connecting to conversion server" << std::endl;
	cppsock::error_t err = car_con.connect("127.0.0.1", 10000);
	std::cout << "connect return value: " << cppsock::strerror(err) << std::endl;
	if (err < 0)
	{
		std::cerr << "Error connecting to conversion server: " << cppsock::strerror(err) << std::endl;
		throw std::logic_error("Error connection to conversion server");
	}

	std::thread _answer_handler(print_answers, &car_con, &running);
	car_con.send_aq(true);

	std::cout << "[send]: help" << std::endl;
	car_con.send_str("help");

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	running = false;
	car_con.close();
	_answer_handler.join();
	return 0;
}
#endif
