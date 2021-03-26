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

#include "SvVis_cortex.hpp"

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

void motor_init(void);
bool motor_cmd_str(const char* cmd, SvVis_t *src);
bool motor_cmd_bin(motor_cmd_bin_t cmd, uint32_t time);

void motor_update_speed(void);
void motor_set_speed(float new_speed);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MOTOR_DRIVER_H
