#ifndef SvVIS3_CORTEX_HPP_INCLUDED
#define SvVIS3_CORTEX_HPP_INCLUDED

// RTOS API here: http://www.keil.com/pack/doc/CMSIS/RTOS2/html/index.html

#include "config.hpp"
#include <string.h>
#include <cstdlib>

#include "misc.h"
#include "cmsis_os2.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#include "COMM_driver.hpp"

#ifndef nullptr
#define nullptr NULL
#endif // nullptr

typedef uint8_t SvVis_channel_t;
struct SvVis_message_t
{
    SvVis_channel_t channel;
    int8_t len;
    union data
    {
        int16_t i16;
        int32_t i32;
        float f;
        char raw[SvVIS_DATA_MAX_LEN];
    } data;
    bool is_string() {return this->channel == SvVIS_STRING_CHANNEL;}
};

struct SvVis_data_t
{
    struct SvVis3_data_tuple_t
    {
        int16_t i16;
        int32_t i32;
        float f;
    };
    SvVis3_data_tuple_t ch[SvVis_CHANNEL_COUNT];
    void apply_msg(SvVis_message_t &msg);
};

class SvVis_t
{
public:
    USART_TypeDef *port;
    //osThreadId_t thread_usart;
    osThreadId_t thread_recv, thread_send;
    osMessageQueueId_t queue_recv, queue_send;
    ring_pipe *pipe_data;
    
    bool init(USART_TypeDef *port, uint32_t baud);

    void send_msg(SvVis_message_t &msgbuf);
    void send_str(const char *str);
    void send_i16(SvVis_channel_t channel, int16_t data);
    void send_i32(SvVis_channel_t channel, int32_t data);
    void send_float(SvVis_channel_t channel, float data);
    uint32_t available();
    void recv_msg(SvVis_message_t &msgbuf);
};

extern SvVis_t *_usart1_handler, *_usart2_handler, *_usart3_handler;

//__NO_RETURN void usart_thread(void *arg);
__NO_RETURN void recv_thread(void *arg);
__NO_RETURN void send_thread(void *arg);

#endif // SvVIS3_CORTEX_HPP_INCLUDED
