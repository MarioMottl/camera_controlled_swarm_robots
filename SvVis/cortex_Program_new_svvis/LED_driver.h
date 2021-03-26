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

/**
 * @brief initialise the LED GPIO Pins
 * 
 */
void LED_init(void);
/**
 * @brief set the heartbeat LED to a fixed state, will be overwritten by the heartbeat thread
 * 
 * @param newstate true if the LED sould be switched on
 */
void LED_heartbeat(bool newstate);
/**
 * @brief set the triangle LEDs, true corresponds to LED ON
 * 
 * @param led0 new status for triangle LED #0
 * @param led1 new status for triangle LED #1
 * @param led2 new status for triangle LED #2
 */
void LED_triangle_l(bool led0, bool led1, bool led2);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LED_DRIVER_HPP_INCLUDED
