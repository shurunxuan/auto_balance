/*
 * motor.h
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This header file includes the declarations
 *  used to drive the motors.
 *         Despite the PWM used by TB6612 and controlled
 *  by TIMER_A1, there are two other interrupts used
 *  here.
 *         The PORT5_IRQHandler and PORT6_IRQHandler
 *  is used for edge detection. The encoders are
 *  connected to P5.4, P5.5 and P6.4, P6.5.
 *         TIMER32 is used to get the counts of the
 *  encoder in a period of time, which is showed in
 *  T32_INT1_IRQHandler.
 *
 */
#include "motor.h"

int encoder_count_1;
int encoder_count_2;

int motor_1_speed;
int motor_2_speed;

int8_t motor_1_direction = 0;
int8_t motor_2_direction = 0;

/* Timer_A UpDown Configuration Parameter */
const Timer_A_UpDownModeConfig upDownConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // 127 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value

};

/* Timer_A Compare Configuration Parameter  (PWM1) */
Timer_A_CompareModeConfig compareConfig_PWM1 =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        TIMER_PERIOD                                // Duty Cycle
};

/* Timer_A Compare Configuration Parameter (PWM2) */
Timer_A_CompareModeConfig compareConfig_PWM2 =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_2,          // Use CCR2
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        TIMER_PERIOD                                // Duty Cycle
};

// ADDED
/* Timer_A Continuous Mode Configuration Parameter */
//const Timer_A_ContinuousModeConfig continuousModeConfig =
//{
//        TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
//        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // SMCLK/1 = 3MHz
//        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
//        TIMER_A_SKIP_CLEAR                   // Skip Clear Counter
//};
//
///* Timer_A Capture Mode Configuration Parameter */
//const Timer_A_CaptureModeConfig captureModeConfig =
//{
//        TIMER_A_CAPTURECOMPARE_REGISTER_3,        // CC Register 3
//        TIMER_A_CAPTUREMODE_RISING_EDGE,          // Rising Edge and falling
//        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,        // CCIxA Input Select
//        TIMER_A_CAPTURE_SYNCHRONOUS,              // Synchronized Capture
//        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,  // Enable interrupt
//        TIMER_A_OUTPUTMODE_OUTBITVALUE            // Output bit value
//};
// -ADDED


void setup_pwm(void)
{
    encoder_count_1 = 0;
    encoder_count_2 = 0;
    motor_1_speed = 0;
    motor_2_speed = 0;

    /* Port mapper configuration register */
    const uint8_t port_mapping[] =
    {
    //Port P2:
    PM_TA1CCR1A, PM_NONE, PM_TA1CCR2A, PM_NONE, PM_TA1CCR2A, PM_NONE, PM_TA1CCR1A, PM_NONE
    };


    /* Remapping TACCR0 to P2.0 */
    MAP_PMAP_configurePorts((const uint8_t *) port_mapping, P2MAP, 1, PMAP_DISABLE_RECONFIGURATION);

    /*
     * P4.0 - STBY
     * P4.2 - AIN1
     * P4.4 - AIN2
     * P4.5 - BIN1
     * P4.7 - BIN2
     */
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);

    /*
     * P4.1 - STATE
     * P4.3 - IR - LEFT
     * P4.6 - IR - RIGHT
     */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN1 | GPIO_PIN3 | GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    /*
     * P5.4 - Encoder_1 A
     * P5.5 - Encoder_1 B
     * P6.4 - Encoder_2 A
     * P6.5 - Encoder_2 B
     */

    // Set input interrupt for P5.4 & P5.5
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
//    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN4);

    // Set input interrupt for P6.4 & P6.5
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4 | GPIO_PIN5);
//    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4 );
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4 | GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
//    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4 | GPIO_PIN5);

    // Set input interrupt for P1.1 & P1.4 (Two Buttons)
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

    /* Enabling interrupts for P5 & P6 */
    MAP_Interrupt_enableInterrupt(INT_PORT5);
    MAP_Interrupt_enableInterrupt(INT_PORT6);

    /* P3.6 - Encoder 1 Direction
     * P3.7 - Encoder 2 Direction */
    MAP_GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // ADDED
    /* Configuring P6.6 as peripheral input for capture */
//    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6,
//        GPIO_PRIMARY_MODULE_FUNCTION);
//
//    /* Configuring Capture Mode */
//    Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig);
//
//    /* Configuring Continuous Mode */
//    Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);
//    /* Enabling interrupts */
//    Interrupt_enableInterrupt(INT_TA2_N);
//
//    /* Starting the Timer_A0 in continuous mode */
//    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);
    // -ADDED

    // Configuring Timer32 to 0.01s in periodic mode
    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);

    // Enabling interrupts for Timer32
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);


    // 3000000 -> 0.25s
    // 12000000 -> 1s
    MAP_Timer32_setCount(TIMER32_BASE, 600000);
//    MAP_Timer32_setCount(TIMER32_BASE, 240000);
    MAP_Timer32_enableInterrupt(TIMER32_BASE);
    MAP_Timer32_startTimer(TIMER32_BASE, false);

    /* Configuring GPIO2.0, GPIO2.1 and GPIO2.2 as peripheral output for PWM */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN6,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring Timer_A1 for UpDown Mode and starting */
    MAP_Timer_A_configureUpDownMode(TIMER_A1_BASE, &upDownConfig);
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UPDOWN_MODE);

    /* Initialize compare registers to generate PWM1 */
    MAP_Timer_A_initCompare(TIMER_A1_BASE, &compareConfig_PWM1);

    /* Initialize compare registers to generate PWM2 */
    MAP_Timer_A_initCompare(TIMER_A1_BASE, &compareConfig_PWM2);
}

void set_pwm_1_duty(uint_fast16_t duty)
{
    /* Timer_A Compare Configuration Parameter  (PWM1) */
    compareConfig_PWM1.compareValue = TIMER_PERIOD - duty;

    /* Initialize compare registers to generate PWM1 */
    MAP_Timer_A_initCompare(TIMER_A1_BASE, &compareConfig_PWM1);
}

void set_pwm_2_duty(uint_fast16_t duty)
{
    /* Timer_A Compare Configuration Parameter (PWM2) */
    compareConfig_PWM2.compareValue = TIMER_PERIOD - duty;

    /* Initialize compare registers to generate PWM2 */
    MAP_Timer_A_initCompare(TIMER_A1_BASE, &compareConfig_PWM2);
}

void set_pwm_duty(uint_fast16_t duty_1, uint_fast16_t duty_2)
{
    set_pwm_1_duty(duty_1);
    set_pwm_2_duty(duty_2);
}

void start_motor(void)
{
    // Set P4.0 high
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
}

void stop_motor(void)
{
    // Set P4.0 low
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
}

void set_motor_run(bool run)
{
    if (run)
        start_motor();
    else
        stop_motor();
}
/*
 * P4.0 - STBY
 * P4.2 - AIN1
 * P4.4 - AIN2
 * P4.5 - BIN1
 * P4.7 - BIN2
 */

void set_motor_1_forward(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);
}

void set_motor_1_backward(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4);
}

void set_motor_1_direction(bool direction)
{
    if (direction)
        set_motor_1_forward();
    else
        set_motor_1_backward();
}

void set_motor_2_forward(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
}

void set_motor_2_backward(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
}

void set_motor_2_direction(bool direction)
{
    if (direction)
        set_motor_2_forward();
    else
        set_motor_2_backward();
}

int get_motor_1_speed(void)
{
    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN6))
    {
        motor_1_direction = 1;
    }
    else
    {
        motor_1_direction = -1;
    }
    return motor_1_direction * motor_1_speed;
    //return Timer_A_getCaptureCompareCount(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3);
}
int get_motor_2_speed(void)
{
    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7))
    {
        motor_2_direction = 1;
    }
    else
    {
        motor_2_direction = -1;
    }
    return motor_2_direction * motor_2_speed;
}

/* GPIO ISR */
void PORT5_IRQHandler(void)
{
    uint16_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5,status);
    ++encoder_count_1;
}

void PORT6_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6,status);
    ++encoder_count_2;
}

void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1,status);
    start_motor();
}

/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_BASE);

    // Toggle P1.0 output
    GPIO_toggleOutputOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    motor_1_speed = encoder_count_1;
    encoder_count_1 = 0;
//    if(motor_1_direction>=0)
//    {
//        motor_1_direction_late=1;
//    }
//    else
//    {
//        motor_1_direction_late=-1;
//    }
//    if(motor_1_speed==0) motor_1_direction_late=0;
//    motor_1_direction=0;
//    motor_1_direction_late = motor_1_direction;

    motor_2_speed = encoder_count_2;
    encoder_count_2 = 0;
//    if(motor_2_direction>=0)
//    {
//        motor_2_direction_late=1;
//
//    }
//    else
//    {
//        motor_2_direction_late=-1;
//
//    }
//    if(motor_2_speed==0)
//        motor_2_direction_late=0;
//    motor_2_direction=0;
//    motor_2_direction_late = motor_2_direction;
}

// ADDED
//void TA2_N_IRQHandler(void)
//{
//    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
//            TIMER_A_CAPTURECOMPARE_REGISTER_3);
//
//
//    motor_1_speed = Timer_A_getCaptureCompareCount(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3);
//}
// -ADDED
