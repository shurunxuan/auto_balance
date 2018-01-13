/*
 * uart.h
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This header file includes the definitions
 *  of usage of UART.
 *
 */

#ifndef ___UART_H
#define ___UART_H

/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void setup_uart(void);
void send_string(const char* string, const uint16_t max_length);

#endif
