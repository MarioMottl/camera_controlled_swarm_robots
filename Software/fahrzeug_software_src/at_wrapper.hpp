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
#include "USART.hpp"
#include "string.h"

namespace AT
{
    const char hostname[] = "ESP8266";
    class wrapper : public serial::interface
    {
    private:
        usart::usart *_interface;
        osThreadId_t _handler;
        /**
         * @brief handle the communication with the ESP8266 module
         * 
         * @param this_void this ptr
         */
        static void handler(void *this_void);
    protected:
        /**
         * @brief run a AT command
         * 
         * @param cmd command to run, does not need to include the end-of-command sequence
         */
        void run_cmd(const char *cmd);
        /**
         * @brief Get the AT response
         * 
         * @param resbuf pointer to the output buffer
         * @param bufsize size of the output buffer
         * @param timeout 
         * @return osStatus_t 
         */
        osStatus_t get_response(char *resbuf, size_t bufsize, uint32_t timeout);
    public:
        /**
         * @brief initialise the AT command wrapper
         * 
         * @param raw raw usart interface
         * @return true init successful
         * @return false init unsuccessful
         */
        bool init(usart::usart &raw);
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
