/**
 *  this file handles basic USART interactions and provides message queues to get raw USART data
 *  The queues are !NOT! initialised in this file, they are initialised in the file "SvVis_cortex.cpp"
 *  This file initialises the Port lines with the correct IO settings and enables the Interrupt service routines to store received data in the queue
 *  Communication without overhead like the HC06 module or the DAPlink adapter can read data directly from these queues
 * 
 *  The SvVis class is responsible for initialising the USART driver
 */

#include "COMM_driver.hpp"

/**
 *  The usart queues are ALWAYS the raw data, whereas the WLAN queue is the queue for decoded received WLAN data
 */
ring_pipe pipe_usart1, pipe_usart2, pipe_usart3;

extern "C" void USART1_IRQHandler(void)
{
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART1);
    pipe_usart1.put(input, 0);
    return;
}

extern "C" void USART2_IRQHandler(void)
{
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART2);
    pipe_usart2.put(input, 0);
    return;
}

extern "C" void USART3_IRQHandler(void)
{
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    char input;
    input = USART_ReceiveData(USART3);
    pipe_usart3.put(input, 0);
    return;
}

void USART_send_bytes(USART_TypeDef* port, const void *start, size_t len)
{
    for(size_t i=0; i<len; i++)
        USART_send_byte(port, ((char*)start)[i] );
}
void USART_send_byte(USART_TypeDef* port, uint8_t byte)
{
    while(USART_GetFlagStatus(port, USART_FLAG_TC) == RESET) {osThreadYield();}
    USART_SendData(port, byte);
}


void init_usart(USART_TypeDef *usartn, uint32_t baud)
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
        // init USART1 RX(PA10) and TX(PA9)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOA, &TX);

        pipe_usart1.init(SvVIS_USART_BACKLOG);

        nvic.NVIC_IRQChannel = USART1_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else if(usartn == USART2)
    {
        // init USART2 RX(PA3) and TX(PA2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init(GPIOA, &TX);

        pipe_usart2.init(SvVIS_USART_BACKLOG);

        nvic.NVIC_IRQChannel = USART2_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else if(usartn == USART3)
    {
        // init USART3 RX(PB11) and TX(PB10)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &RX);
        TX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOB, &TX);

        pipe_usart3.init(SvVIS_USART_BACKLOG);

        nvic.NVIC_IRQChannel = USART3_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    }
    else
    {
        // no usart found, dun't run the code
        return;
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

    /*if(usartn == USART1)      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    else if(usartn == USART2) USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    else if(usartn == USART3) USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);*/
    USART_ITConfig(usartn, USART_IT_RXNE, ENABLE);

	USART_Cmd(usartn, ENABLE);
}
