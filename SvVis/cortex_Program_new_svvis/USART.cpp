/**
 * @file USART.cpp
 * @author Pruggmayer Clemens
 * @brief Implementation for USART Interface implementing the serial::interface
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "USART.hpp"

namespace usart
{
    // global variables from header file
    // becuase inline variables don't work
    // with uvision c++ compiler
    namespace usart1
    {
        ring_pipe pipe;
        ::usart::usart *handler = nullptr;
    } // namespace usart1
    namespace usart2
    {
        ring_pipe pipe;
        ::usart::usart *handler = nullptr;
    } // namespace usart2
    namespace usart3
    {
        ring_pipe pipe;
        ::usart::usart *handler = nullptr;
    } // namespace usart3
} // namespace usart

// ISR for USART1, put received byte into input queue for usart1
extern "C" void USART1_IRQHandler(void)
{
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART1);
    usart::usart1::pipe.put(input, 0);
    return;
}

// ISR for USART2, put received byte into input queue for usart2
extern "C" void USART2_IRQHandler(void)
{
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART2);
    usart::usart2::pipe.put(input, 0);
    return;
}

// ISR for USART3, put received byte into input queue for usart3
extern "C" void USART3_IRQHandler(void)
{
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART3);
    usart::usart3::pipe.put(input, 0);
    return;
}

osStatus_t usart::usart::pop(uint8_t &data, uint32_t timeout)
{
    // get a byte from the input queue, recv_byte is set in the init function
    return this->recv_pipe->pop(data, timeout);
}

osStatus_t usart::usart::put(uint8_t data, uint32_t timeout)
{
    while(USART_GetFlagStatus(this->usartn, USART_FLAG_TC) == RESET) {osThreadYield();}
    USART_SendData(this->usartn, data);
    return osOK;
}

osStatus_t usart::usart::flush(void)
{    
    return osOK;
}

void usart::usart::put_blocking(const void *data, size_t len)
{
    uint8_t *buf = (uint8_t*)data;
    for(size_t i=0; i<len; i++)
    {
        this->put(buf[i], osWaitForever);
    }
}

bool usart::usart::init(USART_TypeDef *usartn, uint32_t baud, size_t recv_pipe_size)
{
    GPIO_InitTypeDef RX, TX;
    USART_InitTypeDef usart;
    USART_ClockInitTypeDef usart_clock;
    NVIC_InitTypeDef nvic;

    SystemCoreClockUpdate();

    TX.GPIO_Mode =  GPIO_Mode_AF_PP;
    RX.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    RX.GPIO_Speed = TX.GPIO_Speed = GPIO_Speed_50MHz;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    if(usartn == USART1)
    {
        if(::usart::usart1::handler != nullptr) { return false; }
        // init USART1 RX(PA10) and TX(PA9)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOA, &TX);

        this->recv_pipe = &::usart::usart1::pipe; // set recv pipe
        ::usart::usart1::handler = this;    // set handler

        nvic.NVIC_IRQChannel = USART1_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else if(usartn == USART2)
    {
        if(::usart::usart2::handler != nullptr) { return false; }
        // init USART2 RX(PA3) and TX(PA2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init(GPIOA, &TX);

        this->recv_pipe = &::usart::usart2::pipe; // set recv pipe
        ::usart::usart2::handler = this;    // set handler

        nvic.NVIC_IRQChannel = USART2_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else if(usartn == USART3)
    {
        if(::usart::usart3::handler != nullptr) { return false; }
        // init USART3 RX(PB11) and TX(PB10)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &RX);
        TX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOB, &TX);

        this->recv_pipe = &::usart::usart3::pipe; // set recv pipe
        ::usart::usart3::handler = this;    // set handler

        nvic.NVIC_IRQChannel = USART3_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else
    {
        // no usart found, dun't run the code
        return false;
    }

    USART_DeInit(usartn);

    usart_clock.USART_Clock = USART_Clock_Disable;
	usart_clock.USART_CPOL = USART_CPOL_Low;
	usart_clock.USART_CPHA = USART_CPHA_2Edge;
	usart_clock.USART_LastBit = USART_LastBit_Disable;
 
	USART_ClockInit(usartn, &usart_clock);

    usart.USART_BaudRate = baud;
	usart.USART_WordLength = USART_WordLength_8b;	 
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(usartn, &usart);
    NVIC_Init(&nvic);

    USART_ITConfig(usartn, USART_IT_RXNE, ENABLE);

    this->recv_pipe->init(recv_pipe_size);
    this->usartn = usartn;
    this->done_sending = true;

	USART_Cmd(usartn, ENABLE);
    return true;
}
