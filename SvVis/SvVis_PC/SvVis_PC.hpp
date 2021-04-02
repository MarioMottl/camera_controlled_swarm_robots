#pragma once

#include "../../library/cppsock/cppsock.hpp"
#include <queue>

namespace SvVis
{
	constexpr size_t data_max_len = 32;
	using channel_t = uint8_t;
	using flags_t = uint32_t;

	/**
	 * @brief namespace for channel muber constants
	 */
	namespace channel
	{
		constexpr size_t count = 9;
		constexpr channel_t string = 10;
		constexpr channel_t int16_base = 11;
		constexpr channel_t float_base = 21;
	} // namespace channel

	/**
	 * @brief struct for the SvVis message packet
	 */
	struct message_t
	{
		channel_t channel;
		size_t len;
		union data
		{
			int16_t i16;
			float f;
			char raw[SvVis::data_max_len];
		} data;
		bool is_string()
		{
			return this->channel == SvVis::channel::string;
		}
	}; // struct message_t

	class client
	{
	protected:
		cppsock::tcp::client sock;
		//std::queue<SvVis::message_t> queue_recv;
		std::mutex mutex_send, mutex_recv;
		/**
		 * @brief converts a channel number into the respective maximum data length
		 * @return length of the associated data, maximum data length for string messages
		 * @return 0 if the channel has no data associated with it
		 */
		static size_t chid2len(uint8_t chid)
		{
			if (chid == ::SvVis::channel::string) return ::SvVis::data_max_len;
			if ((chid >= ::SvVis::channel::float_base) && (chid < ::SvVis::channel::float_base + ::SvVis::channel::count)) return sizeof(float);
			if ((chid >= ::SvVis::channel::int16_base) && (chid < ::SvVis::channel::int16_base + ::SvVis::channel::count)) return sizeof(int16_t);
			return 0;
		}
	public:
		/**
		 * @brief connect to a server to communicate with the µC counterpart
		 */
		cppsock::error_t connect(const cppsock::socketaddr& addr)
		{
			return this->sock.connect(addr);
		}
		/**
		 * @brief connect to a server to communicate with the µC counterpart
		 */
		cppsock::error_t connect(const char* hostname, const char* service)
		{
			return this->sock.connect(hostname, service);
		}
		/**
		 * @brief connect to a server to communicate with the µC counterpart
		 */
		cppsock::error_t connect(const char* hostname, uint16_t port)
		{
			return this->sock.connect(hostname, port);
		}

		/**
		 * @brief sends an aq event. true = enable sending, false = disable sending
		 */
		void send_aq(bool aq_state)
		{
			if (aq_state)
			{
				this->sock.send("\x0A""\x01""\x00", 3, 0); // aq on sequence
			}
			else
			{
				this->sock.send("\x0A""\x00""\x00", 3, 0); // aq off sequence
			}
		}
		/**
		 * @brief send a message to the µC counterpart
		 */
		void send_msg(const SvVis::message_t& msg)
		{
			std::unique_lock<std::mutex> _lock(this->mutex_send); // mutex every send operation
			this->sock.send(&msg.channel, sizeof(SvVis::channel_t), 0);
			this->sock.send(&msg.data, msg.len, 0);
		}
		/**
		 * @brief send a string message to the µC counterpart
		 */
		void send_str(const std::string& str)
		{
			SvVis::message_t msg;
			memset(&msg, 0, sizeof(msg));
			msg.channel = SvVis::channel::string;
			msg.len = str.length() + 1; // +1 because of '\0'
			str.copy(msg.data.raw, sizeof(msg.data.raw));
			this->send_msg(msg);
		}
		/**
		 * @brief receives a message
		 * blocks until a message has been received
		 * @return false if the underlying socket has been closed / shut down
		 */
		bool recv_msg(SvVis::message_t& msg)
		{
			uint8_t recvbuf;
			size_t maxlen;
			// init length
			msg.len = 0;
			if (this->sock.recv(&recvbuf, sizeof(recvbuf), 0) <= 0) // if the socket is closed, return false
				return false;
			msg.channel = recvbuf;
			maxlen = SvVis::client::chid2len(msg.channel);
			memset(&msg.data, 0, sizeof(msg.data));
			if (msg.channel != ::SvVis::channel::string)
			{
				// handle non-string messages
				while (msg.len < maxlen)
				{
					this->sock.recv(&recvbuf, sizeof(recvbuf), 0);
					if (msg.len < SvVis::data_max_len) { msg.data.raw[msg.len++] = recvbuf; }
				}
				return true;
			}
			else
			{
				// handle string message
				while (recvbuf != '\0')
				{
					this->sock.recv(&recvbuf, sizeof(recvbuf), 0); // the terminating '\0' will be put into the buffer
					if (msg.len < ::SvVis::data_max_len - 1) { msg.data.raw[msg.len++] = recvbuf; }
				}
				msg.data.raw[::SvVis::data_max_len - 1] = '\0'; // security cut at the end of the string
				return true;
			}
		}

		/**
		 * @brief closes the connection
		 * after a close() - call, recv_msg will return false as soon as there are no messages to receive
		 */
		void close()
		{
			this->sock.close();
		}
	}; // class client
} // namespace SvVis
