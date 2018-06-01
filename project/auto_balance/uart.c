/*
 * uart.c
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This header file includes the declarations
 *  of usage of UART.
 *
 */

/* Specific Defines */
#include "uart.h"
#include "protocol.h"

#define MAX_RXDATA_LENGTH 100
#define END_CHAR '$'

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 38400 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // SMCLK Clock Source
        78,                                             // BRDIV = 78
        2,                                              // UCxBRF = 2
        0,                                              // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                         // No Parity
        EUSCI_A_UART_LSB_FIRST,                         // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
        EUSCI_A_UART_MODE,                              // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION   // Oversampling
};

uint8_t RXData[MAX_RXDATA_LENGTH];
uint16_t RXData_length = 0;

void setup_uart(void)
{
    // Selecting P1.2 and P1.3 in UART mode
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                    GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Setting DCO to 12MHz
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    // Configuring UART Module for PC
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // Enable UART module for PC
    MAP_UART_enableModule(EUSCI_A0_BASE);

    // Enabling interrupts for PC
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);

    // Configuring UART Module for BT
    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);

    // Enable UART module for BT
    MAP_UART_enableModule(EUSCI_A2_BASE);

    // Enabling interrupts for BT
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
}

void setup_bluetooth(void)
{
    //send_string_bt("AT+BAUD6\r\n", 12);
    //send_string_bt("AT+PARI0\r\n", 12);
    //send_string_bt("AT+NAMEBALANCE_CAR\r\n", 25);
    //send_string_bt("AT+PIN123456\r\n", 20);
    //send_string_bt("AT+LADDR\r\n", 20);
    //send_string_bt("AT+UART=9600,0,0\r\n", 100);
}

// A simple helper function to send a string through UART
void send_string_pc(const char* string, const uint16_t max_length)
{
    uint16_t pos;
    for (pos = 0; pos < max_length; ++pos)
    {
        if (string[pos] == 0)
            break;
        MAP_UART_transmitData(EUSCI_A0_BASE, string[pos]);
    }
}

void send_string_bt(const char* string, const uint16_t max_length)
{
    uint16_t pos;
    for (pos = 0; pos < max_length; ++pos)
    {
        if (string[pos] == 0)
            break;
        MAP_UART_transmitData(EUSCI_A2_BASE, string[pos]);
    }
}

void send_char_pc(const char ch)
{
    MAP_UART_transmitData(EUSCI_A0_BASE, ch);
}

void send_char_bt(const char ch)
{
    MAP_UART_transmitData(EUSCI_A2_BASE, ch);
}

/* EUSCI A0 UART ISR */
// Not used yet.
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char received_data = MAP_UART_receiveData(EUSCI_A0_BASE);
        //send_char_bt(received_data);
    }
}

/* EUSCI A2 UART ISR */
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char received_data = MAP_UART_receiveData(EUSCI_A2_BASE);
        //send_char_pc(received_data);
        if (received_data == END_CHAR)
        {
            process_command(RXData_length, RXData);
            RXData_length = 0;
            memset(RXData, 0x00, MAX_RXDATA_LENGTH);
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        }
        else
        {
            RXData[RXData_length] = received_data;
            ++RXData_length;
        }
    }
}
