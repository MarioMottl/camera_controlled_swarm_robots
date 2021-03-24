#ifndef LED_DRIVER_HPP_INCLUDED
#define LED_DRIVER_HPP_INCLUDED

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "cmsis_os2.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void LED_init(void);

void LED_heartbeat(bool newstate);

void LED_triangle_l(bool led0, bool led1, bool led2);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LED_DRIVER_HPP_INCLUDED
