/**
 * @file at_wrapper.cpp
 * @author Pruggmayer Clemens
 * @brief implementation for the AT command wrapper
 * @version 0.1
 * @date 2021-03-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "at_wrapper.hpp"

void AT::wrapper::handler(void *this_void)
{
    char response[128];
    ::AT::wrapper *tar = (::AT::wrapper*)this_void;
    tar->_interface->enable_recv(false); // disable receiving
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &gpio);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET); // enable wlan module
    osDelay(100); // wait for the WLAN module to finish sending stuff
    tar->run_cmd("ATE0"); // disable AT echoing
    osDelay(5); // wait for the disabled echo to be processed properly
    tar->_interface->enable_recv(true); // clear all received data

    // Set ESP module into Station mode
    strncpy(response, "AT+CWMODE_CUR=1", sizeof(response) );
    tar->run_cmd(response);
    tar->get_response(nullptr, 0, osWaitForever); // empty line
    tar->get_response(nullptr, 0, osWaitForever); // OK

    // Set ESP hostname
    //osDelay(1); // delay to let the ESP module enable Station mode
    strncpy(response, "AT+CWHOSTNAME=\"", sizeof(response) );
    strncat(response, AT::hostname, sizeof(response)-1 - strlen(response) );
    strncat(response, "\"", sizeof(response)-1 - strlen(response) );
    tar->run_cmd(response);
    tar->get_response(nullptr, 0, osWaitForever); // empty line
    tar->get_response(nullptr, 0, osWaitForever); // OK

    // Set to single connection mode
    strncpy(response, "AT+CIPMUX=0", sizeof(response) );
    tar->run_cmd(response);
    tar->get_response(nullptr, 0, osWaitForever); // empty line
    tar->get_response(nullptr, 0, osWaitForever); // OK
}

void AT::wrapper::run_cmd(const char *cmd)
{
    this->_interface->put_blocking(cmd, strlen(cmd));
    this->_interface->put_blocking("\r\n", 2);
}

osStatus_t AT::wrapper::get_response(char *resbuf, size_t bufsize, uint32_t timeout)
{
    uint8_t recvbuf;
    size_t i=0;
    if(this->_interface->pop(recvbuf, timeout) == osErrorTimeout) return osErrorTimeout; // get first character and handle timeout
    do
    {
        if(i < bufsize) // buffer size limitation
            if(resbuf != nullptr)
                resbuf[i++] = recvbuf;
        // if statement to handle empty responses (\r\n\r\n)
        if(recvbuf != '\r') this->_interface->pop(recvbuf, osWaitForever); // get next character
    }
    while(recvbuf != '\r'); // as long as the received input is not \r
    if(resbuf != nullptr) {resbuf[i] = '\0';} // set \r to \0
    this->_interface->pop(recvbuf, osWaitForever); // remove \n
    return osOK;
}

bool AT::wrapper::init(usart::usart &interface)
{
    this->_interface = &interface;
    this->_handler = osThreadNew(::AT::wrapper::handler, this, nullptr);
    return true;
}

osStatus_t AT::wrapper::pop(uint8_t &data, uint32_t timeout)
{
    return osError;
}
osStatus_t AT::wrapper::put(uint8_t data, uint32_t timeout)
{
    return osError;
}
osStatus_t AT::wrapper::flush(void)
{
    return osError;
}
void       AT::wrapper::put_blocking(const void *data, size_t len)
{
    return;
}
