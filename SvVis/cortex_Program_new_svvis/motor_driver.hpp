#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "cmsis_os2.h"
#include "misc.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SvVis.hpp"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//typedef uint8_t motor_cmd_bin_t;
typedef enum
{
    MOTOR_CMD_STOP,
    MOTOR_CMD_FW,
    MOTOR_CMD_BW,
    MOTOR_CMD_RR,
    MOTOR_CMD_RL
} motor_cmd_bin_t;

/**
 * @brief initialise the motor GPIO Pins
 * 
 */
void motor_init(void);
/**
 * @brief run a motor command and provide a answer interface
 * 
 * @param cmd motor command in string format
 * @param src pointer to the answer interface
 * @return true command recognized and executed
 * @return false command not recognized
 */
bool motor_cmd_str(const char* cmd, SvVis::SvVis *src);
/**
 * @brief run a motor command and specify a stop time
 * 
 * @param cmd motor command from the motor command enum
 * @param time after how many units the ommand should stop (~mm for fw/bw, ~° for rr/rl)
 * @return true command recognized and executed
 * @return false command not recognized
 */
bool motor_cmd_bin(motor_cmd_bin_t cmd, uint32_t time);

/**
 * @brief update the motor speed
 * 
 */
void motor_update_speed(void);
/**
 * @brief set the relative motor speed
 * 
 * @param new_speed 
 */
void motor_set_speed(float new_speed);
/**
 * @brief stop the motors
 * 
 */
void motor_stop(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MOTOR_DRIVER_H
