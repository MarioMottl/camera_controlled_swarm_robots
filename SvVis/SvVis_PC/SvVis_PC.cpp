#include "SvVis_PC.hpp"

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
