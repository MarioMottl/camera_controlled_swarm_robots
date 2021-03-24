#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/**
 *  This file contains several configuration constants
 */

#include <stdint.h>

#define SvVIS_DATA_MAX_LEN 32
#define SvVIS_USART_BACKLOG 64
#define SvVIS_WLAN_BACKLOG 64
#define SvVIS_RECV_BACKLOG 4
#define SvVIS_SEND_BACKLOG 4

#define SvVis_CHANNEL_COUNT 9
#define SvVIS_I32_CHANNEL_BASE 1
#define SvVIS_I16_CHANNEL_BASE 11
#define SvVIS_STRING_CHANNEL 10
#define SvVIS_FLOAT_CHANNEL_BASE 21

#define USART_BAUD_9600 (9600 * 3) /* *3 becuase of some error with the hardware I could not fix */
#define USART_BAUD_115200 (115200 * 3) /* *3 becuase of some error with the hardware I could not fix */
#define USART_BAUD USART_BAUD_115200

#define WLAN_HOSTNAME "ESP8266"
#define WLAN_SSID "<ssid>"
#define WLAN_PWD "<pwd>"
#define WLAN_IP "<ip>"
#define WLAN_PORT "<port>"

#endif // CONFIG_H_INCLUDED
