/**
 *  This file is responsible for interfacing the ESP-8266
 *  This file takes the raw data from the queue from the USART driver, and puts the decoded data into a seperate message queue
 * 
 *  The SvVis class is responsible for initialising the USART driver AND WLAN driver
 */

#include "COMM_driver.hpp"
#include "extra.hpp"

#include <stdio.h>

//ring_pipe pipe_wlan_data_input, pipe_wlan_output_data;

osThreadId_t thread_parse;
osMutexId_t mutex_usart;
ring_pipe pipe_wlan_data_input;

__NO_RETURN void _WLAN_connecter(void *arg)
{
    bool connected = false;
    char response[128];

    pipe_wlan_data_input.init(SvVIS_WLAN_BACKLOG);

    // USART connection via USART3 is ready to use
    pipe_usart3.enaple_put(false); // disable usart3 input
    // WLAN enable Port: PA5
    GPIO_InitTypeDef wlan_en;
    wlan_en.GPIO_Mode = GPIO_Mode_Out_PP;
    wlan_en.GPIO_Speed = GPIO_Speed_2MHz;
    wlan_en.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &wlan_en);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET); // enable wlan module
    osDelay(100); // wait for the WLAN module to finish sending stuff
    _WLAN_run_cmd("ATE0"); // disable AT echoing
    osDelay(5); // wait for the disabled echo to be processed properly
    pipe_usart3.enaple_put(true); // reenable usart3 input

    // Set ESP module into Station mode
    strncpy(response, "AT+CWMODE_CUR=1", sizeof(response) );
    _WLAN_run_cmd(response);
    __WLAN_get_response(nullptr, 0, osWaitForever); // empty line
    __WLAN_get_response(nullptr, 0, osWaitForever); // OK

    // Set ESP hostname
    //osDelay(1); // delay to let the ESP module enable Station mode
    strncpy(response, "AT+CWHOSTNAME=\"", sizeof(response) );
    strncat(response, WLAN_HOSTNAME, sizeof(response)-1 - strlen(response) );
    strncat(response, "\"", sizeof(response)-1 - strlen(response) );
    _WLAN_run_cmd(response);
    __WLAN_get_response(nullptr, 0, osWaitForever); // empty line
    __WLAN_get_response(nullptr, 0, osWaitForever); // OK

    // Set to single connection mode
    strncpy(response, "AT+CIPMUX=0", sizeof(response) );
    _WLAN_run_cmd(response);
    __WLAN_get_response(nullptr, 0, osWaitForever); // empty line
    __WLAN_get_response(nullptr, 0, osWaitForever); // OK
                
    // set to TCP receive mode to passive mode
    strncpy(response, "AT+CIPRECVMODE=0", sizeof(response) );
    _WLAN_run_cmd(response);
    __WLAN_get_response(nullptr, 0, osWaitForever); // empty line
    __WLAN_get_response(nullptr, 0, osWaitForever); // OK
    // connect to WLAN Access point
    strncpy(response, "AT+CWJAP_CUR=\"", sizeof(response) );
    strncat(response, WLAN_SSID, sizeof(response)-1 - strlen(response) );
    strncat(response, "\",\"", sizeof(response)-1 - strlen(response) );
    strncat(response, WLAN_PWD, sizeof(response)-1 - strlen(response) );
    strncat(response, "\"", sizeof(response)-1 - strlen(response) ); 
    _WLAN_run_cmd(response);
    __WLAN_get_response(nullptr, 0, osWaitForever); // WIFI connected
    __WLAN_get_response(nullptr, 0, osWaitForever); // WIFI IP got
    __WLAN_get_response(nullptr, 0, osWaitForever); // \r\n
    __WLAN_get_response(nullptr, 0, osWaitForever); // OK

    mutex_usart = osMutexNew(nullptr);

    for(;;)
    {
        osMutexAcquire(mutex_usart, osWaitForever);
        do
        {
            while(!connected) // reconnect if neccesary
            {
                // connect to the TCP server
                strncpy(response, "AT+CIPSTART=\"TCP\",\"", sizeof(response) );
                strncat(response, WLAN_IP, sizeof(response)-1 - strlen(response) );
                strncat(response, "\",", sizeof(response)-1 - strlen(response) );
                strncat(response, WLAN_PORT, sizeof(response)-1 - strlen(response) );
                _WLAN_run_cmd(response);
                __WLAN_get_response(nullptr, 0, osWaitForever); // CONNECT / ERROR
                __WLAN_get_response(nullptr, 0, osWaitForever); // \r\n
                __WLAN_get_response(response, sizeof(response), osWaitForever); // OK / CLOSED
                connected = (strncmp(response, "OK", 2) == 0); // if OK is responded, we are connected
                if(!connected) {osDelay(1000);} // delay connect tries to reduce network and USART stress
            }
            while(__WLAN_get_response(response, sizeof(response), 10) != osErrorTimeout)
            {
                if(strncmp(response, "CLOSED", 6) == 0) // if CLOSED is responded, the connection is closed
                {
                    connected = false;
                }
                else if(strncmp(response, "\r", 1) == 0)
                {
                    uint8_t recvbuf;
                    size_t i, recv_size;
                    // remove responce header (+IPD,) from input pipe
                    // check response header
                    pipe_usart3.pop(recvbuf, osWaitForever);if(recvbuf != '+') continue;
                    pipe_usart3.pop(recvbuf, osWaitForever);if(recvbuf != 'I') continue;
                    pipe_usart3.pop(recvbuf, osWaitForever);if(recvbuf != 'P') continue;
                    pipe_usart3.pop(recvbuf, osWaitForever);if(recvbuf != 'D') continue;
                    pipe_usart3.pop(recvbuf, osWaitForever);if(recvbuf != ',') continue;
                    //for(i=0; i<5; i++) pipe_usart3.pop(recvbuf, osWaitForever);
                    i=0;
                    // retrieve data length
                    while(recvbuf != ':')
                    {
                        pipe_usart3.pop(recvbuf, osWaitForever);
                        response[i++] = recvbuf;
                    }
                    response[i] = '\0';
                    recv_size = strtoul(response, nullptr, 0);
                    for(i=0; i<recv_size; i++)
                    {
                        pipe_usart3.pop(recvbuf, osWaitForever);
                        pipe_wlan_data_input.put(recvbuf, osWaitForever);
                    }
                }
            }
        }
        while(!connected); // ensure that we are connected when the mutex is released
        osMutexRelease(mutex_usart);
        osDelay(1); // wait a bit to give other threads a chance to send data
    }
}

void WLAN_init()
{
    thread_parse= osThreadNew(_WLAN_connecter, nullptr, nullptr);
}

osStatus_t __WLAN_get_response(char *resbuf, size_t bufsize, uint32_t timeout)
{   
    uint8_t recvbuf;
    size_t i=0;
    if(pipe_usart3.pop(recvbuf, timeout) == osErrorTimeout) return osErrorTimeout; // get first character and handle timeout
    do
    {
        if(i < bufsize) // buffer size limitation
            if(resbuf != nullptr)
                resbuf[i++] = recvbuf;
        // if statement to handle empty responses (\r\n\r\n)
        if(recvbuf != '\r') pipe_usart3.pop(recvbuf, osWaitForever); // get next character
    }
    while(recvbuf != '\r'); // as long as the received input is not \r
    if(resbuf != nullptr) {resbuf[i] = '\0';} // set \r to \0
    pipe_usart3.pop(recvbuf, osWaitForever); // remove \n
    return osOK;
}

void _WLAN_run_cmd(const char *cmd)
{
    USART_send_bytes(USART3, cmd, strlen(cmd)); // the command itself
    USART_send_bytes(USART3, "\r\n", 2); // terminates the command sequence
}

void WLAN_send_msg(struct SvVis_message_t &msg)
{
    char sendbuf[sizeof(msg.channel)+sizeof(msg.data)];
    memcpy(sendbuf, &msg.channel, sizeof(msg.channel));
    memcpy(sendbuf+sizeof(msg.channel), &msg.data, msg.len);
    WLAN_send_bytes(sendbuf, msg.len+sizeof(msg.channel) );
}
void WLAN_send_byte(uint8_t byte)
{
    WLAN_send_bytes(&byte, sizeof(byte));
}
void WLAN_send_bytes(void *start, size_t len)
{
    char cmdbuf[128];
    //uint8_t recvbuf;
    osMutexAcquire(mutex_usart, osWaitForever);
        memset(cmdbuf, 0, sizeof(cmdbuf) ); // set entire command to zeros
        strncpy(cmdbuf, "AT+CIPSENDBUF=", sizeof(cmdbuf) );
        ul_to_string(cmdbuf + strlen(cmdbuf), sizeof(cmdbuf)-1 - strlen(cmdbuf), len);
        _WLAN_run_cmd(cmdbuf);
        __WLAN_get_response(nullptr, 0, osWaitForever); // 2,1
        __WLAN_get_response(nullptr, 0, osWaitForever); // \r\n
        __WLAN_get_response(nullptr, 0, osWaitForever); // OK
        //pipe_usart3.pop(recvbuf, osWaitForever); // '>'
        //pipe_usart3.pop(recvbuf, osWaitForever); // ' '
        USART_send_bytes(USART3, start, len); // send data bytes
        //__WLAN_get_response(nullptr, 0, osWaitForever); // Recv x bytes
    osMutexRelease(mutex_usart);
    return;
}
