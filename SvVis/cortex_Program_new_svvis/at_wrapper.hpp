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
        bool init(serial::interface &raw);
        virtual osStatus_t pop(uint8_t &data, uint32_t timeout);
        virtual osStatus_t put(uint8_t data, uint32_t timeout);
        virtual osStatus_t flush(void);
        virtual void       put_blocking(const void *data, size_t len);
    };
} // namespace AT
