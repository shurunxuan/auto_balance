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

#define MAX_RXDATA_LENGTH 100
#define END_CHAR '$'

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
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
uint16_t RX_pos = 0;

void setup_uart(void)
{
    // Selecting P1.2 and P1.3 in UART mode
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    // Setting DCO to 12MHz
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    // Configuring UART Module
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // Enable UART module
    MAP_UART_enableModule(EUSCI_A0_BASE);

    // Enabling interrupts
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
}

// A simple helper function to send a string through UART
void send_string(const char* string, const uint16_t max_length)
{
    uint16_t pos;
    for (pos = 0; pos < max_length; ++pos)
    {
        if (string[pos] == 0)
            break;
        MAP_UART_transmitData(EUSCI_A0_BASE, string[pos]);
    }
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
    }
}
