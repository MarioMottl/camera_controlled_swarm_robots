#include "SvVis.hpp"

bool SvVis::message_t::is_string()
{
    return this->channel == ::SvVis::channel::string;
}

bool SvVis::SvVis::init(serial::interface &interface, size_t send_queue_size, size_t recv_queue_size)
{
    if(this->interface != nullptr) { return false; }
    this->interface = &interface;
    this->queue_send = osMessageQueueNew(send_queue_size, sizeof(::SvVis::message_t), nullptr);
    this->queue_recv = osMessageQueueNew(recv_queue_size, sizeof(::SvVis::message_t), nullptr);
    this->thread_send = osThreadNew(::SvVis::SvVis::func_send, this, nullptr);
    this->thread_recv = osThreadNew(::SvVis::SvVis::func_recv, this, nullptr);
    this->event_flags = osEventFlagsNew(nullptr);

    // setting event flag default
    osEventFlagsSet  (this->event_flags, ::SvVis::flags::done_sending); // default: no message, done sending
    osEventFlagsSet  (this->event_flags, ::SvVis::flags::n_flushing); // defualt: not flushing
    osEventFlagsClear(this->event_flags, ::SvVis::flags::aq_on); // default: aq off
    return true;
}

osStatus_t SvVis::SvVis::send_msg(::SvVis::message_t &msgbuf, uint32_t timeout)
{
    //if(this->flushing == true) { return osErrorResource; }
    if( (osEventFlagsGet(this->event_flags) & ::SvVis::flags::aq_on) == 0 ) {return osErrorResource;} // aq off, block messages from being put into the queue
    if(osEventFlagsWait(this->event_flags, ::SvVis::flags::n_flushing, osFlagsWaitAny | osFlagsNoClear, timeout) == (uint32_t)osErrorTimeout) { return osErrorTimeout; }
    //this->done_sending = false;
    osEventFlagsClear(this->event_flags, ::SvVis::flags::done_sending);
    return osMessageQueuePut(this->queue_send, &msgbuf, 0, timeout); // put message into queue
}

osStatus_t SvVis::SvVis::send_str(const char *str, uint32_t timeout)
{
    size_t len = strlen(str)+1;
    if(len >= ::SvVis::data_max_len) {len = ::SvVis::data_max_len;}
    ::SvVis::message_t msg;
    msg.channel = ::SvVis::channel::string;
    msg.len = len;
    memcpy(msg.data.raw, str, len);
    msg.data.raw[::SvVis::data_max_len-1] = '\0';
    return this->send_msg(msg, timeout);
}

osStatus_t SvVis::SvVis::send_i16(::SvVis::channel_t channel, int16_t data, uint32_t timeout)
{
    size_t len = sizeof(data);
    ::SvVis::message_t msg;
    msg.channel = ::SvVis::channel::int16_base + channel;
    msg.len = len;
    msg.data.i16 = data;
    return this->send_msg(msg, timeout);
}

osStatus_t SvVis::SvVis::send_float(::SvVis::channel_t channel, float data, uint32_t timeout)
{
    size_t len = sizeof(data);
    ::SvVis::message_t msg;
    msg.channel = ::SvVis::channel::float_base + channel;
    msg.len = len;
    msg.data.f = data;
    return this->send_msg(msg, timeout);
}

osStatus_t SvVis::SvVis::flush()
{
    //this->flushing = true;
    osEventFlagsClear(this->event_flags, ::SvVis::flags::n_flushing);
    osEventFlagsWait(this->event_flags, ::SvVis::flags::done_sending, osFlagsWaitAny, osWaitForever); // wait for send queue to be empty
    //while ( this->done_sending  == false ) { osThreadYield(); }
    
    this->interface->flush(); // wait for interface to be empty
    //this->flushing = false;
    osEventFlagsSet(this->event_flags, ::SvVis::flags::n_flushing);
    return osOK;
}

bool SvVis::SvVis::aq_enabled()
{
    return osEventFlagsGet(this->event_flags) & ::SvVis::flags::aq_on != 0;
}

void SvVis::SvVis::aq_set(bool newstate)
{
    if(newstate)
        osEventFlagsSet(this->event_flags, ::SvVis::flags::aq_on);
    else
        osEventFlagsClear(this->event_flags, ::SvVis::flags::aq_on);
}

size_t SvVis::SvVis::available()
{
    return osMessageQueueGetCount(this->queue_recv);
}

void SvVis::SvVis::recv_msg(::SvVis::message_t &msgbuf, uint32_t timeout)
{
    osMessageQueueGet(this->queue_recv, &msgbuf, nullptr, timeout);
}
