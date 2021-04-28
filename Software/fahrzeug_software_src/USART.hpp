/**
 * @file USART.hpp
 * @author Pruggmayer Clemens
 * @brief Definition for USART Interface implementing the serial::interface
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "serial_interface.hpp"
#include "ring_pipe.hpp"

namespace usart
{
    class usart : public serial::interface
    {
    private:
        USART_TypeDef *usartn;
        osThreadId_t send_thread;
        ring_pipe *recv_pipe;
        bool done_sending;
    public:
        /**
         * @brief initialise the USART Interface
         * HINT: on some MDDS Systems, the baudrate might be wrong by a factor of 3 
         * 
         * @param usartn usart define constant from the STD-Peripheral Library
         * @param baud baud rate, might be wrong by a factor of 3
         * @param recv_pipe_size length of the receive pipe
         * @return true init successful
         * @return false init unsuccessful
         */
        bool init(USART_TypeDef *usartn, uint32_t baud, size_t recv_pipe_size = 64);
        /**
         * @brief enable receiving data
         * 
         * @param newstate true to enable receiving
         * @return osStatus_t 
         */
        osStatus_t enable_recv(bool newstate);
        /**
         * @brief get one byte from the input queue
         * 
         * @param data where to put the data
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t pop(uint8_t &data, uint32_t timeout);
        /**
         * @brief put one byte to the serial interface, in this class, this function in unbuffered
         * 
         * @param data what to send
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t put(uint8_t data, uint32_t timeout);
        /**
         * @brief wait until everything in sent, this will return immediatley, since this class has unbuffered output
         * 
         * @return osStatus_t 
         */
        virtual osStatus_t flush(void);
        /**
         * @brief put multiple bytes out of the USART interface
         * 
         * @param data start of memory
         * @param len length of memory
         */
        virtual void put_blocking(const void *data, size_t len);
    };

    namespace usart1
    {
        extern ring_pipe queue;
        extern ::usart::usart *handler;
    } // namespace usart1
    namespace usart2
    {
        extern ring_pipe queue;
        extern ::usart::usart *handler;
    } // namespace usart2
    namespace usart3
    {
        extern ring_pipe queue;
        extern ::usart::usart *handler;
    } // namespace usart3

    namespace flags
    {
        const uint32_t done_sending = 0x00000001;
    } // namespace flags   
} // namespace usart
