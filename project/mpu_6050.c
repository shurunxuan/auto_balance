/*
 * mpu_6050.c
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This file includes the declarations
 *  of MPU-6050 relative functions and types.
 *
 */

// Specific Defines
#include "mpu_6050.h"

// Slave Address for I2C Slave
// For MPU-6050, this is 0x68
#define SLAVE_ADDRESS       (0x68)
// Read two bytes at a time
#define NUM_OF_REC_BYTES    2

// Gravitational acceleration
#define G 9.794f

// MPU-6050 requires 14 bytes of data
/* 0x3B, Acceleration of X axis: ACC_X
   0x3D, Acceleration of Y axis: ACC_Y
   0x3F, Acceleration of Z axis: ACC_Z
   0x41, Current Temperature: TEMP
   0x43, Angular speed of X axis: GYR_X
   0x45, Angular speed of Y axis: GYR_Y
   0x47, Angular speed of Z axis: GYR_Z */
// First addresses of 7 registers
const uint8_t RegData[] = {0x3B, 0x3D, 0x3F, 0x41, 0x43, 0x45, 0x47};
// I2C Read data buffer
static uint8_t RXData[NUM_OF_REC_BYTES];
// I2C Read data index
static volatile uint32_t xferIndex;
// Flag that indicates the I2C read data complete
static volatile int8_t data_ready;
// Current ACC ratio of MPU-6050
static MPU_ACC_CONF acc_conf_ = ACC_2G;
const float ACC_RATIO[] = {2.0f, 4.0f, 8.0f, 16.0f};
// Current GYR ratio of MPU-6050
static MPU_GYR_CONF gyr_conf_ = GYR_250;
const float GYR_RATIO[] = {250.0f, 500.0f, 1000.0f, 2000.0f};

// I2C Master Configuration Parameter
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

// Setup I2C Communication
void setup_i2c(void)
{
    // Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
    //   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    data_ready = 0;
    memset(RXData, 0x00, NUM_OF_REC_BYTES);

    // Initializing I2C Master to SMCLK at 100khz with no autostop
    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);

    // Specify slave address
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDRESS);

    // Enable I2C Module to start operations
    MAP_I2C_enableModule(EUSCI_B0_BASE);
    MAP_Interrupt_enableInterrupt(INT_EUSCIB0);
}

// Read data through I2C from specified register
// Data saved in RXData
void i2c_read(uint8_t reg_addr)
{
    data_ready = 0;

    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    MAP_Interrupt_enableSleepOnIsrExit();

    /* Send the register address */
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, reg_addr);

    /* Sent the address, now we need to initiate the read */
    xferIndex = 0;
    MAP_I2C_masterReceiveStart(EUSCI_B0_BASE);
    MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    MAP_PCM_gotoLPM0InterruptSafe();

    // Wait for the data is ready
    while (data_ready == 0);
}

// Wake up MPU-6050
void setup_mpu_6050(void)
{
    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    MAP_Interrupt_enableSleepOnIsrExit();

    /* Writing 0x00 to register 0x6B */
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x6B);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, 0x00);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
}

// Setup ratio for MPU-6050 data
void setup_ratio(MPU_ACC_CONF acc_conf, MPU_GYR_CONF gyr_conf)
{
    uint8_t acc_reg;
    uint8_t gyr_reg;
    // The register address of acceleration is 0x1C
    //i2c_read(0x1C);
    acc_reg = (char)acc_conf << 3;

    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    MAP_Interrupt_enableSleepOnIsrExit();

    /* Writing acc_reg to register 0x1C */
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x1C);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, acc_reg);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);

    // Set the global configure
    acc_conf_ = acc_conf;

    // The gyro address of acceleration is 0x1B
    //i2c_read(0x1B);
    gyr_reg = (char)gyr_conf << 3;

    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

    MAP_Interrupt_enableSleepOnIsrExit();

    /* Writing gyr_reg to register 0x1B */
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x1B);
    MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, gyr_reg);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);

    // Set the global configure
    gyr_conf_ = gyr_conf;
}

void request_mpu_data(MPU_DATA* data)
{
    int16_t data_array[7];
    int8_t i;
    for (i = 0; i < 7; ++i)
    {
        uint16_t data_u = 0;

        data_ready = 0;

        /* Making sure the last transaction has been completely sent out */
        while (MAP_I2C_masterIsStopSent(EUSCI_B0_BASE));

        MAP_Interrupt_enableSleepOnIsrExit();

        /* Send the data address to MPU-6050 */
        MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, RegData[i]);

        /* Sent the address, now we need to initiate the read */
        xferIndex = 0;
        MAP_I2C_masterReceiveStart(EUSCI_B0_BASE);
        MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

        MAP_PCM_gotoLPM0InterruptSafe();

        // Wait for the data is ready
        while (data_ready == 0);

        // Combine 2 bytes into an int16_t
        data_u = RXData[0];
        data_u = (data_u << 8) | RXData[1];
        data_array[i] = *((int16_t*)((uint16_t*)(&data_u)));
    }
    float acc_r_temp = ACC_RATIO[(uint8_t)acc_conf_] * G / 32768.0f;
    float gyr_r_temp = GYR_RATIO[(uint8_t)gyr_conf_] / 32768.0f;
    data->ACC_X = acc_r_temp * data_array[1];
    data->ACC_Y = acc_r_temp * data_array[2];
    data->ACC_Z = acc_r_temp * data_array[3];
    data->TEMP  = data_array[4];
    data->GYR_X = gyr_r_temp * data_array[5];
    data->GYR_Y = gyr_r_temp * data_array[6];
    data->GYR_Z = gyr_r_temp * data_array[0];
}

/*******************************************************************************
 * eUSCIB0 ISR. The repeated start and transmit/receive operations happen
 * within this ISR.
 *******************************************************************************/
void EUSCIB0_IRQHandler(void)
{
    uint_fast16_t status;
    status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B0_BASE);
    MAP_I2C_clearInterruptFlag(EUSCI_B0_BASE, status);

    /* Receives bytes into the receive buffer. If we have received all bytes,
     * send a STOP condition */
    if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
    {
        if (xferIndex == NUM_OF_REC_BYTES - 2)
        {
            MAP_I2C_disableInterrupt(EUSCI_B0_BASE,
                    EUSCI_B_I2C_RECEIVE_INTERRUPT0);
            MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_STOP_INTERRUPT);

            /*
             * Switch order so that stop is being set during reception of last
             * byte read byte so that next byte can be read.
             */
            MAP_I2C_masterReceiveMultiByteStop(EUSCI_B0_BASE);
            RXData[xferIndex++] = MAP_I2C_masterReceiveMultiByteNext(
                    EUSCI_B0_BASE);
            data_ready = 1;
        }
        else
        {
            RXData[xferIndex++] = MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B0_BASE);
        }
    }
    else if (status & EUSCI_B_I2C_STOP_INTERRUPT)
    {
        MAP_Interrupt_disableSleepOnIsrExit();
        MAP_I2C_disableInterrupt(EUSCI_B0_BASE,
                                 EUSCI_B_I2C_STOP_INTERRUPT);
    }
}
