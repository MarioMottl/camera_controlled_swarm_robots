/**
 *  This file handles the SvVis communication protocol together with "SvVis_cortex_threads"
 *  To receive raw data, the message queues provided by the USART and WLAN driver are used
 *  message queues are used to be able to process a different data source which uses some overhead (WLAN)
 *  The class uses message queues to send / receive data which are continusly processed by the thread functions in "SvVis_cortex_threads.cpp"
 * 
 *  This class is responsible for initialising the underlying drivers for the communication
 */

#include "SvVis_cortex.hpp"

/**
 *  Only one instance should control a USART port, these pointers can also be used as global reference to a specific port
 */
SvVis_t *_usart1_handler = nullptr, *_usart2_handler = nullptr, *_usart3_handler = nullptr;

bool SvVis_t::init(USART_TypeDef *port, uint32_t baud)
{
    if(port == USART1)
    {
        if(_usart1_handler != nullptr) return false;
        _usart1_handler = this;
        // for usart1 (daplink), no overhead is used, so the raw usart pipe can be used
        this->pipe_data = &pipe_usart1;
    }
    else if(port == USART2)
    {
        if(_usart2_handler != nullptr) return false;
        _usart2_handler = this;
        //for usart2 (bluetooth HC06), no overhead is used, so the raw usart pipe can be used
        this->pipe_data = &pipe_usart2;
    }
    else if(port == USART3)
    {
        if(_usart3_handler != nullptr) return false;
        _usart3_handler = this;
        // pipe_usart3 contains the raw communication with the WLAN module (with overhead), a seperate pipe for processed data is used instead
        this->pipe_data = &pipe_wlan_data_input;
    }
    else
    {
        return false;
    }
    init_usart(port, baud); // initialise USART and USART interrupts
    this->port = port;
    
    this->queue_send  = osMessageQueueNew(SvVIS_SEND_BACKLOG, sizeof(SvVis_message_t), nullptr); // message queue for messages to be sent
    this->queue_recv  = osMessageQueueNew(SvVIS_RECV_BACKLOG, sizeof(SvVis_message_t), nullptr); // message queue for messages to receive

    this->thread_recv = osThreadNew(recv_thread, this, nullptr); // thread to receive raw bytes and decode into messages
    this->thread_send = osThreadNew(send_thread, this, nullptr); // thread to send messages from queue
    return true;
}

void SvVis_t::send_msg(SvVis_message_t &message)
{
    while(osMessageQueueGetSpace(this->queue_send) < 2) {osThreadYield();} // wait for available space in send queue
    osMessageQueuePut(this->queue_send, &message, nullptr, osWaitForever);
}
void SvVis_t::send_str(const char *str)
{
    // prepares message for string and caps the string at the maximum length
    size_t len = strlen(str)+1;
	bool too_long = (len >= SvVIS_DATA_MAX_LEN);
    if(too_long) {len = SvVIS_DATA_MAX_LEN;}
    SvVis_message_t message;
    message.channel = SvVIS_STRING_CHANNEL;
    message.len = len;
    memcpy(message.data.raw, str, len);
	if(too_long) {message.data.raw[SvVIS_DATA_MAX_LEN-1] = 0; /*USART_send_byte(USART1, '#');*/} else {/*USART_send_byte(USART1, '!');*/}
    this->send_msg(message);
}
void SvVis_t::send_i16(SvVis_channel_t channel, int16_t data)
{
    // prepares message for int16_t
    size_t len = sizeof(data);
    SvVis_message_t message;
    message.channel = SvVIS_I16_CHANNEL_BASE + channel;
    message.len = len;
    message.data.i16 = data;
    this->send_msg(message);
}
void SvVis_t::send_i32(SvVis_channel_t channel, int32_t data)
{
    // prepares message for int32_t, it is known that this function does not work with SvVis Form App for windows
    size_t len = sizeof(data);
    SvVis_message_t message;
    message.channel = SvVIS_I32_CHANNEL_BASE + channel;
    message.len = len;
    message.data.i32 = data;
    this->send_msg(message);
}
void SvVis_t::send_float(SvVis_channel_t channel, float data)
{
    // prepares message for float
    size_t len = sizeof(data);
    SvVis_message_t message;
    message.channel = SvVIS_FLOAT_CHANNEL_BASE + channel;
    message.len = len;
    message.data.f = data;
    this->send_msg(message);
}

uint32_t SvVis_t::available()
{
    return osMessageQueueGetCount(this->queue_recv);
}

void SvVis_t::recv_msg(SvVis_message_t &msg_buf)
{
    osMessageQueueGet(this->queue_recv, &msg_buf, NULL, osWaitForever);
}

void SvVis_data_t::apply_msg(SvVis_message_t &msg)
{
    if( (msg.channel >= SvVIS_I16_CHANNEL_BASE) && (msg.channel < SvVIS_I16_CHANNEL_BASE + SvVis_CHANNEL_COUNT) ) // 11..19
    {
        this->ch[msg.channel - SvVIS_I16_CHANNEL_BASE].i16 = msg.data.i16;
        this->ch[msg.channel - SvVIS_I16_CHANNEL_BASE].i32 = msg.data.i16;
        this->ch[msg.channel - SvVIS_I16_CHANNEL_BASE].f   = msg.data.i16;
    }
    else if( (msg.channel >= SvVIS_I32_CHANNEL_BASE) && (msg.channel < SvVIS_I32_CHANNEL_BASE + SvVis_CHANNEL_COUNT) ) // 1..9
    {
        this->ch[msg.channel - SvVIS_I32_CHANNEL_BASE].i16 = msg.data.i32;
        this->ch[msg.channel - SvVIS_I32_CHANNEL_BASE].i32 = msg.data.i32;
        this->ch[msg.channel - SvVIS_I32_CHANNEL_BASE].f   = msg.data.i32;
    }
    else if( (msg.channel >= SvVIS_FLOAT_CHANNEL_BASE) && (msg.channel < SvVIS_FLOAT_CHANNEL_BASE + SvVis_CHANNEL_COUNT) ) // 21..29
    {
        this->ch[msg.channel - SvVIS_FLOAT_CHANNEL_BASE].i16 = msg.data.f;
        this->ch[msg.channel - SvVIS_FLOAT_CHANNEL_BASE].i32 = msg.data.f;
        this->ch[msg.channel - SvVIS_FLOAT_CHANNEL_BASE].f   = msg.data.f;
    }
}
