#include "SvVis.hpp"

namespace SvVis
{
    /**
     * @brief convert a channel number to a size
     * 
     * @param chid channel number
     * @return size_t length of the data for the channel number
     */
    size_t chid2len(uint8_t chid)
    {
        if(chid == ::SvVis::channel::string) return ::SvVis::data_max_len;
        if((chid >= ::SvVis::channel::float_base) && (chid < ::SvVis::channel::float_base + ::SvVis::channel::count)) return sizeof(float);
        if((chid >= ::SvVis::channel::int16_base) && (chid < ::SvVis::channel::int16_base + ::SvVis::channel::count)) return sizeof(int16_t);
        return 0;
    }
} // namespace SvVis

__NO_RETURN void SvVis::SvVis::func_recv(void *this_void)
{
    ::SvVis::SvVis *tar = (::SvVis::SvVis*)this_void;
    ::SvVis::message_t msg;
    uint8_t recvbuf;
    size_t maxlen;
    for(;;)
    {
        // init length
        msg.len = 0;
        tar->interface->pop(recvbuf, osWaitForever);
        msg.channel = recvbuf;
        maxlen = ::SvVis::chid2len(msg.channel);
        memset(&msg.data, 0, sizeof(msg.data));
        if(msg.channel != ::SvVis::channel::string)
        {
            // handle non-string messages
            while (msg.len < maxlen)
            {
                tar->interface->pop(recvbuf, osWaitForever);
                if(msg.len < ::SvVis::data_max_len) {msg.data.raw[msg.len++] = recvbuf;}
            }
            osMessageQueuePut(tar->queue_recv, &msg, 0, osWaitForever);
        }
        else
        {
            // handle string message
            while (recvbuf != '\0')
            {
                tar->interface->pop(recvbuf, osWaitForever); // the terminating '\0' will be put into the buffer
                if(msg.len < ::SvVis::data_max_len-1) {msg.data.raw[msg.len++] = recvbuf;}
            }
            msg.data.raw[::SvVis::data_max_len-1] = '\0'; // security cut at the end of the string
            
            if(msg.data.i16 == 0)
            {   // aq off
                osEventFlagsClear(tar->event_flags, ::SvVis::flags::aq_on);
                osMessageQueueReset(tar->queue_send); // clear message queue
            }
            else if(msg.data.i16 == 1)
            {   // aq on
                osEventFlagsSet(tar->event_flags, ::SvVis::flags::aq_on);
            }
            else
            {   // string message
                osMessageQueuePut(tar->queue_recv, &msg, 0, osWaitForever);
            }
        }
    }
}
__NO_RETURN void SvVis::SvVis::func_send(void *this_void)
{
    ::SvVis::SvVis *tar = (::SvVis::SvVis*)this_void;
    ::SvVis::message_t msg;
    for(;;)
    {
        osMessageQueueGet(tar->queue_send, &msg, nullptr, osWaitForever);
        osEventFlagsWait(tar->event_flags, ::SvVis::flags::aq_on, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
        tar->interface->put(msg.channel, osWaitForever);
        tar->interface->put_blocking(msg.data.raw, msg.len);
        if(osMessageQueueGetCount(tar->queue_send) == 0) { osEventFlagsSet(tar->event_flags, ::SvVis::flags::done_sending); /*tar->done_sending = true;*/ }
    }
}
