/**
 * @file at_wrapper.hpp
 * @author Pruggmayer Clemens
 * @brief definition for the AT command wrapper for the ESP8266 WLAN module
 * @version 0.1
 * @date 2021-03-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "serial_interface.hpp"

namespace AT
{
    class wrapper : public serial::interface
    {
    private:
        serial::interface *_raw;
    public:
        /**
         * @brief initialise the AT command wrapper
         * 
         * @param raw raw usart interface
         * @return true init successful
         * @return false init unsuccessful
         */
        bool init(serial::interface &raw);
        /**
         * @brief get one byte from the TCP input
         * 
         * @param data where to write the data
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t pop(uint8_t &data, uint32_t timeout);
        /**
         * @brief send one byte to the TCP output
         * 
         * @param data what to send
         * @param timeout 
         * @return osStatus_t 
         */
        virtual osStatus_t put(uint8_t data, uint32_t timeout);
        /**
         * @brief wait until all bytes are transferred to the TCP output, this function returns immediatley since this class has unbuffered output
         * 
         * @return osStatus_t 
         */
        virtual osStatus_t flush(void);
        /**
         * @brief put multiple bytes into the TCP output
         * 
         * @param data start of memory
         * @param len length of data
         */
        virtual void       put_blocking(const void *data, size_t len);
    };
} // namespace AT
