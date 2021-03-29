#pragma once

#include "../../../library/cppsock/cppsock.hpp"

namespace SvVis
{
	class client
	{
	protected:
		cppsock::tcp::client sock;
	public:
		bool connect(cppsock::socketaddr& addr);
		bool connect(const char* hostname, const char* service);
		bool connect(const char* hostname, uint16_t port);

		void close();
	}; // class client
} // namespace SvVis