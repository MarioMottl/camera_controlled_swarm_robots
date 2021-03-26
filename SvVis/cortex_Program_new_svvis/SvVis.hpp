
#pragma once

#include <stdint.h>
#include <string.h>

#include "serial_interface.hpp"

namespace SvVis
{
    // type definitions and constants
    const size_t data_max_len = 32;

    typedef uint8_t channel_t;
    namespace channel
    {
        const size_t             count      = 9;
        const ::SvVis::channel_t string     = 10;
        const ::SvVis::channel_t int16_base = 11;
        const ::SvVis::channel_t float_base = 21;
    } // namespace channel

    namespace priority
    {
        const size_t none = 0;
        const size_t max = 0xFFFFFFFF;
    } // namespace priority
    
    namespace flags
    {
        const uint32_t done_sending = 0x00000001;
        const uint32_t n_flushing   = 0x00000002;
        const uint32_t aq_on        = 0x00000004;
    } // namespace flags

    // message type
    struct message_t
    {
        ::SvVis::channel_t channel;
        int8_t len;
        union data
        {
            int16_t i16;
            float   f;
            char    raw[::SvVis::data_max_len];
        } data;
        bool is_string();
    };

    // SvVis protocol handler
    class SvVis
    {
    private:
        serial::interface *interface;
        osMessageQueueId_t queue_send, queue_recv;
        osThreadId_t thread_send, thread_recv;
        osEventFlagsId_t event_flags;
        //bool done_sending, flushing;
        /**
         * @brief continuus thread function to send data
         * 
         * @param this_void this as void*
         * @return __NO_RETURN 
         */
        static __NO_RETURN void func_send(void *this_void);
        /**
         * @brief continuus thread function to receive and interpret data
         * 
         * @param this_void 
         * @return __NO_RETURN 
         */
        static __NO_RETURN void func_recv(void *this_void);
    public:
        SvVis() : interface(nullptr)/*, done_sending(true), flushing(false)*/ {} // constructor because uvision c++ compiler cant automatically initialise member data
        /**
         * @brief initialise the ahndler
         * 
         * @param interface serial interface for the unormatted I/O
         * @param send_queue_size size of the receive queue
         * @param recv_queue_size size of the send queue
         * @return true init successful
         * @return false init unsuccessful
         */
        bool init(serial::interface &interface, size_t send_queue_size = 4, size_t recv_queue_size = 4);

        /**
         * @brief scedule a message to send
         * 
         * @param msgbuf the message to send, must be initialised corrctly
         * @param timeout 
         * @return osStatus_t 
         */
        osStatus_t send_msg(::SvVis::message_t &msgbuf, uint32_t timeout = osWaitForever);
        /**
         * @brief scedule a string to send
         * 
         * @param str the string to schedule to send
         * @param timeout 
         * @return osStatus_t 
         */
        osStatus_t send_str(const char *str,  uint32_t timeout = osWaitForever);
        /**
         * @brief scedule a 16-bit integer to send
         * 
         * @param channel channel number [0..8]
         * @param data the integer to schedule to send
         * @param timeout 
         * @return osStatus_t 
         */
        osStatus_t send_i16(::SvVis::channel_t channel, int16_t data, uint32_t timeout = osWaitForever);
        /**
         * @brief scedule a flaot to send
         * 
         * @param channel channel number [0..8]
         * @param data the float to schedule to send
         * @param timeout 
         * @return osStatus_t 
         */
        osStatus_t send_float(::SvVis::channel_t channel, float data, uint32_t timeout = osWaitForever);
        /**
         * @brief FIXME: wait until all messages have been sent
         * 
         * @return osStatus_t 
         */
        osStatus_t flush();
        /**
         * @brief get if the aquisition is enabled by the peer
         * 
         * @return true the connection is enabled
         * @return false the connection should not send data
         */
        bool       aq_enabled();
        /**
         * @brief override the aquisition state
         * 
         * @param newstate true to forcefully enable data sending
         */
        void       aq_set(bool newstate);
        /**
         * @brief get the number of messages available to receive
         * 
         * @return size_t 
         */
        size_t available();
        /**
         * @brief receive a message from the input queue
         * 
         * @param msgbuf buffer to write the message
         * @param timeout 
         */
        void recv_msg(::SvVis::message_t &msgbuf, uint32_t timeout = osWaitForever);
    };
} // namespace SvVis
