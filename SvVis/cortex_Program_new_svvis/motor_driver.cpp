/**
 *  This file is responsible of initialising verything the motors need to run properly
 *  This file is also responsible for handling string/binary commands for the motor
 */

/**
 *  LEFT MOTOR PINS 
 *      PB0         (PWM Signal)
 *      PA6, PA7    (direction control Signals)
 *      PC12, PC13  (Rotary Encoder Signals)
 * 
 *  RIGHT MOTOR PINS
 *      PB1         (PWM Signal)
 *      PA0, PA1    (direction control signals)
 *      PB8, PB9    (Rotary Encoder Signals)
 *
 *  TIM3 CH3, CH4: PWM Generators
 *  TIM5 IRQ: Regulation
 */

#include "motor_driver.hpp"
#include "extra.hpp"

double _speed_mult = 1.0;
double _base_speed_left = 0.1, _base_speed_right = 0.1;
volatile uint32_t _counter_right = 0, _counter_left = 0;
int64_t _counter_ttd = 0; // counter time to drive
const uint16_t _timer_period = 4096;
bool motor_running = false;
osThreadId_t _os_thread_regulation;

uint32_t motor_verbose_output_counter;
const uint32_t motor_verbose_output_period = 1;
SvVis::SvVis *motor_verbose_next_com = nullptr;

extern "C" void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line8) != RESET) // MOTOR A
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        _counter_right++;
    }
}

extern "C" void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12) != RESET) // MOTOR B
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        _counter_left++;
    }
}

extern "C" void TIM5_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); // clear ISR bit
    if(motor_running)
    {
        /*_base_speed_left = 0.1;
        _base_speed_right = (_base_speed_left * _counter_left) / _counter_right;*/
        if(_counter_right > _counter_left)
        {
            _base_speed_left += ( ((double)_counter_right / (double)_counter_left) + 1.0 ) * 0.00001;
            _base_speed_right = 0.1;
        }
        else
        {
            _base_speed_right += ( ((double)_counter_left / (double)_counter_right) + 1.0 ) * 0.00001;
            _base_speed_left = 0.1;
        }

        _counter_ttd -= (_counter_left + _counter_right) / 2;
        if(_counter_ttd <= 0) motor_stop();

        motor_update_speed();
    }
    // motor verbose output
    if(motor_verbose_next_com != nullptr)
    {
        if(motor_verbose_output_counter != 0) motor_verbose_output_counter--;
        else
        {
            motor_verbose_next_com->send_i16(0, _counter_left / 32, 0);
            motor_verbose_next_com->send_i16(1, _counter_right / 32, 0);
            motor_verbose_output_counter = motor_verbose_output_period;
        }
    }
    _counter_left = 1;
    _counter_right = 1;
}

void motor_stop(void)
{
    _counter_ttd = 0;
    _base_speed_left = _base_speed_right = 10.0;
    motor_running = false;
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
    motor_update_speed();
    osDelay(2);
    _base_speed_left = _base_speed_right = 0.1;
    motor_update_speed();
    _counter_left = _counter_right = 1;
}

void timer_init(void)
{
    /**
     *  TIM3 CH3 (PB0): output PWM for motor enable for LEFT  motor
     *  TIM3 CH4 (PB1): output PWM for motor enable for RIGHT motor
     */
    TIM_TimeBaseInitTypeDef timer;
    TIM_OCInitTypeDef outputcompare;
    GPIO_InitTypeDef gpio;
    NVIC_InitTypeDef nvic;

    // set PB0, PB1 as output for motor enable signals
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // enable clock for gpiob
    memset(&gpio, 0, sizeof(gpio));
    gpio.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate function output (Output compare output)
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_0; // PB0
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1; // PB1
    GPIO_Init(GPIOB, &gpio);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enable clock for timer
    memset(&timer, 0, sizeof(timer));
    timer.TIM_RepetitionCounter = 0; // infinite repetition
    timer.TIM_CounterMode = TIM_CounterMode_Up; // count upwards
    timer.TIM_ClockDivision = TIM_CKD_DIV1; // no clock division
    timer.TIM_Prescaler = 16; // prescaler
    timer.TIM_Period = _timer_period; // autoreload
    TIM_TimeBaseInit(TIM3, &timer);

    memset(&outputcompare, 0, sizeof(outputcompare));
    outputcompare.TIM_OCMode = TIM_OCMode_PWM1;
    outputcompare.TIM_OutputState = TIM_OutputState_Enable;
    outputcompare.TIM_Pulse = timer.TIM_Period * _base_speed_left * _speed_mult;
    TIM_OC3Init(TIM3, &outputcompare);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
    outputcompare.TIM_Pulse = timer.TIM_Period * _base_speed_right * _speed_mult;
    TIM_OC4Init(TIM3, &outputcompare);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    memset(&timer, 0, sizeof(timer));
    timer.TIM_RepetitionCounter = 0;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_Prescaler = 1024;
    timer.TIM_Period = 1024;
    TIM_TimeBaseInit(TIM5, &timer);

    memset(&nvic, 0, sizeof(nvic));
    nvic.NVIC_IRQChannel = TIM5_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM5, ENABLE);
}

void motor_init(void)
{
    GPIO_InitTypeDef gpio;
    EXTI_InitTypeDef exti;
    NVIC_InitTypeDef nvic;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    //enable GPIOA, GPIOB, GPIOC and AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    // set PA0,PA1, PA6,PA7 as output for motor control signals
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);

    // set B8, C12 as input for EXTI
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &gpio);

    // init EXTI line 8
    exti.EXTI_Line = EXTI_Line8;
    exti.EXTI_LineCmd = ENABLE;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&exti);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
    EXTI_ClearITPendingBit(EXTI_Line8);
    // init EXTI line 12
    exti.EXTI_Line = EXTI_Line12;
    EXTI_Init(&exti);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);
    EXTI_ClearITPendingBit(EXTI_Line12);

    // init nvic
    nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&nvic);
    nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&nvic);

    // init PWM generators
    timer_init();

    osThreadAttr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.priority = osPriorityAboveNormal;

    //_os_thread_regulation = osThreadNew(motor_feedback_handler, nullptr, &attr);
}

void motor_update_speed(void)
{
    TIM_OCInitTypeDef outputcompare;

    // reinit outputcompare units
    memset(&outputcompare, 0, sizeof(outputcompare));
    outputcompare.TIM_OCMode = TIM_OCMode_PWM1;
    outputcompare.TIM_OutputState = TIM_OutputState_Enable;
    outputcompare.TIM_Pulse = motor_running ? _timer_period * _base_speed_left * _speed_mult : _timer_period;
    TIM_OC3Init(TIM3, &outputcompare); // PB0 PWM Signal

    outputcompare.TIM_Pulse = motor_running ? _timer_period * _base_speed_right * _speed_mult : _timer_period;
    TIM_OC4Init(TIM3, &outputcompare); // PB1 PWM Signal
}

void motor_set_speed(float new_speed)
{
    _speed_mult = new_speed;
    motor_update_speed();
}

bool motor_cmd_str(const char* cmd, SvVis::SvVis *src)
{
    uint32_t time = osWaitForever;
    // after the end of the string, the buffer is consistently filled with zeros thanks to memset() in recv_thread() in SvVis_cortex_threads.cpp
    if( strncmp(cmd, "stop", 4) == 0 )
    {
        time = strtoul(cmd+5, NULL, 0); // "stop %d"
        motor_cmd_bin(MOTOR_CMD_STOP, time);
    }
    else if( strncmp(cmd, "fw", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "fw %d"
        motor_cmd_bin(MOTOR_CMD_FW, time);
    }
    else if( strncmp(cmd, "bw", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "bw %d"
        motor_cmd_bin(MOTOR_CMD_BW, time);
    }
    else if( strncmp(cmd, "rr", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "rr %d"
        motor_cmd_bin(MOTOR_CMD_RR, time);
    }
    else if( strncmp(cmd, "rl", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "rl %d"
        motor_cmd_bin(MOTOR_CMD_RL, time);
    }
    else if( strncmp(cmd, "speed", 5) == 0 ) // set speed
    {
        if(strlen(cmd) == 5)
        {
            char msg[SvVis::data_max_len];
            //snprintf(msg, sizeof(msg), "current speed: %d", (int)(_speed_mult * 128) );
            strcpy(msg, "current speed: ");
            ul_to_string(msg+15, sizeof(msg)-15, _speed_mult*128);
            src->send_str(msg);
        }
        else
        {
            time = strtoul(cmd+6, NULL, 0); // "speed %d"
            motor_set_speed(time / 128.0);
        }
    }
    else if( strncmp(cmd, "out m off", 17) == 0 ) // reset debug output interface
    {
        src->send_str("reset motor output");
        motor_verbose_next_com = nullptr;
    }
    else if( strncmp(cmd, "out m", 13) == 0 ) // set debug output interface
    {
        motor_verbose_next_com = src;
        src->send_str("set motor output");
    }
    else if( strncmp(cmd, "cntr", 4) == 0 ) // print cpunter values
    {
        char msg[::SvVis::data_max_len];
        // "l: %12d"
        // "r: %12d"
        strncpy(msg, "l: ", 3);
        ul_to_string(msg+3, sizeof(msg)-3, _counter_left);
        src->send_str(msg);
        strncpy(msg, "r: ", 3);
        ul_to_string(msg+3, sizeof(msg)-3, _counter_right);
        src->send_str(msg);
        strncpy(msg, "s_l: ", 5);
        ul_to_string(msg+5, sizeof(msg)-3, _base_speed_left*1000);
        src->send_str(msg);
        strncpy(msg, "s_r: ", 5);
        ul_to_string(msg+5, sizeof(msg)-3, _base_speed_right*1000);
        src->send_str(msg);
    }
    else // unrecognized command, return false to indicate unsuccessful command execution
    {
        return false; // return false if an unknown command was used
    }
    return true; // return true if a valid command was received
}

bool motor_cmd_bin(motor_cmd_bin_t cmd, uint32_t time)
{
    switch (cmd)
    {
    case MOTOR_CMD_STOP:
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        motor_stop();
        break;
    case MOTOR_CMD_FW:
        _counter_ttd = time * 256;
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        _counter_left = _counter_right = 1;
        motor_running = true;
        motor_update_speed();
        break;
    case MOTOR_CMD_BW:
        _counter_ttd = time * 256;
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        _counter_left = _counter_right = 1;
        motor_running = true;
        motor_update_speed();
        break;
    case MOTOR_CMD_RR:
        _counter_ttd = time * 30;
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        _counter_left = _counter_right = 1;
        motor_running = true;
        motor_update_speed();
        break;
    case MOTOR_CMD_RL:
        _counter_ttd = time * 30;
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        _counter_left = _counter_right = 1;
        motor_running = true;
        motor_update_speed();
        break;

    default: // unrecognized command, stopping
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        motor_stop();
        return false; // return false if an unrecognized command was used
    }
    return true; // return true if a valid command was used
}
