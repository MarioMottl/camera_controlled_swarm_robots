#ifndef USART_DRIVER_H_INCLUDED
#define USART_DRIVER_H_INCLUDED

#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "cmsis_os2.h"

#include "ring_pipe.hpp"
#include "SvVis_cortex.hpp"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void init_usart(USART_TypeDef *usartn, uint32_t baud);
void USART_send_bytes(USART_TypeDef* port, const void *start, size_t len);
void USART_send_byte(USART_TypeDef* port, uint8_t byte);

extern ring_pipe pipe_usart1, pipe_usart2, pipe_usart3; // message queues for incoming bytes, usart3 contains raw data
extern ring_pipe pipe_wlan_data_input; // message queue for WLAN data Input, output is via WLAN_send_byte[s]()

void WLAN_init();
osStatus_t __WLAN_get_response(char *resbuf, size_t bufsize, uint32_t timeout);
/// cmd does not include end of command sequence
void _WLAN_run_cmd(const char *cmd);
void WLAN_send_msg(struct SvVis_message_t &msg);
void WLAN_send_byte(uint8_t byte);
void WLAN_send_bytes(void *start, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USART_DRIVER_H_INCLUDED
