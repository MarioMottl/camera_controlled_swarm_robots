/**
 * @file serial_interface.hpp
 * @author Pruggmayer Clemens
 * @brief Definition of a serial Interface for sending/received data
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ring_pipe.hpp"

namespace serial
{
    class interface
    {
    public:
        /**
         * @brief get one byte and remove it from the queue
         * 
         * @param data where the data should be written into
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t pop(uint8_t &data, uint32_t timeout) = 0;
        /**
         * @brief put one byte into the queue
         * 
         * @param data what should be put into the queue
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t put(uint8_t data, uint32_t timeout) = 0;
        /**
         * @brief wait until every byte has been pop()-ed
         * 
         * @return osStatus_t 
         */
        virtual osStatus_t flush(void) = 0;
        /**
         * @brief put several bytes into the pipe
         * 
         * @param data pointer to the data
         * @param len length of the data
         */
        virtual void       put_blocking(const void *data, size_t len) = 0;
    };
} // namespace serial
