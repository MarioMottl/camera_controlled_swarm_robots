/**
 *  This file handles the continuusly running threads to decode / send data
 *  Data is sent from a message queue and received data is put into a different message queue
 */

#include "SvVis_cortex.hpp"

uint8_t chid2len(uint8_t chid)
{
    if(chid == SvVIS_STRING_CHANNEL) return SvVIS_DATA_MAX_LEN;
    if((chid >= SvVIS_FLOAT_CHANNEL_BASE) && (chid < SvVIS_FLOAT_CHANNEL_BASE + SvVis_CHANNEL_COUNT)) return sizeof(float);
    if((chid >= SvVIS_I16_CHANNEL_BASE) && (chid < SvVIS_I16_CHANNEL_BASE + SvVis_CHANNEL_COUNT)) return sizeof(int16_t);
    if((chid >= SvVIS_I32_CHANNEL_BASE) && (chid < SvVIS_I32_CHANNEL_BASE + SvVis_CHANNEL_COUNT)) return sizeof(int32_t);
    return 0;
}

__NO_RETURN void recv_thread(void *arg)
{
    SvVis_t *tar = (SvVis_t*)arg;
    SvVis_message_t msgbuf;
    uint8_t recvbuf;
    uint8_t maxlen;
    for(;;)
    {
        // init length number
        msgbuf.len = 0;
        tar->pipe_data->pop(recvbuf, osWaitForever);
        msgbuf.channel = recvbuf;
        maxlen = chid2len(msgbuf.channel);
        // initialise data with zeros
        memset(&msgbuf.data, 0, sizeof(msgbuf.data));
        // get data
        if(msgbuf.channel != SvVIS_STRING_CHANNEL)
        {
            while(msgbuf.len < maxlen) // non-string handling
            {
                tar->pipe_data->pop(recvbuf, osWaitForever);
                // push back recvbuf
                if(msgbuf.len < SvVIS_DATA_MAX_LEN) {msgbuf.data.raw[msgbuf.len++] = recvbuf;}
            }
        }
        else
        {
            while(recvbuf != '\0')
            {
                tar->pipe_data->pop(recvbuf, osWaitForever);
                // push back received character
                if(msgbuf.len < SvVIS_DATA_MAX_LEN-1) {msgbuf.data.raw[msgbuf.len++] = recvbuf;} // write character from input data
            }
            //msgbuf.data.raw[SvVIS_DATA_MAX_LEN-1] ='\0';
        }
        osMessageQueuePut(tar->queue_recv, &msgbuf, nullptr, osWaitForever);
    }
}
__NO_RETURN void send_thread(void *arg)
{
    SvVis_t *tar = (SvVis_t*)arg;
    SvVis_message_t msgbuf;
    for(;;)
    {
        osMessageQueueGet(tar->queue_send, &msgbuf, nullptr, osWaitForever);
        if(tar->port == USART3)
        {
            //WLAN_send_byte(msgbuf.channel);
            //WLAN_send_bytes(&msgbuf.data, msgbuf.len);
            WLAN_send_msg(msgbuf);
        }
        else
        {
            USART_send_byte(tar->port, msgbuf.channel); // send channel byte
            USART_send_bytes(tar->port, &msgbuf.data, msgbuf.len); // send data
        }
    }
}
