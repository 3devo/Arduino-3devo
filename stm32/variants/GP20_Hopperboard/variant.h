/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0                     A0
#define PA1                     A1
#define PA2                     A2
#define PA3                     A3
#define PA4                     A4
#define PA5                     A5
#define PA6                     A6
#define PA7                     A7
#define PA8                     8
#define PA9                     9
#define PA10                    10
#define PA11                    11
#define PA12                    12
#define PA13                    A8
#define PA14                    A9
#define PA15                    15
#define PB0                     A10
#define PB1                     A11
#define PB2                     A12
#define PB3                     19
#define PB4                     20
#define PB5                     21
#define PB6                     22
#define PB7                     23
#define PB8                     24
#define PB9                     25
#define PB10                    A13
#define PB11                    A14
#define PB12                    A15
#define PB13                    29
#define PB14                    30
#define PB15                    31
#define PC6                     32
#define PC7                     33
#define PC13                    34
#define PC14                    35
#define PC15                    36
#define PD0                     37
#define PD1                     38
#define PD2                     39
#define PD3                     40
#define PF0                     41
#define PF1                     42
#if 0
// For when https://github.com/stm32duino/Arduino_Core_STM32/pull/1091
// is merged
#define PA9_R                   43
#define PA10_R                  44

// Alternate pins number
#define PA6_ALT1                (PA6 | ALT1)
#define PA7_ALT1                (PA7 | ALT1)
#define PA7_ALT2                (PA7 | ALT2)
#define PA7_ALT3                (PA7 | ALT3)
#define PB0_ALT1                (PB0 | ALT1)
#define PB1_ALT1                (PB1 | ALT1)
#define PB1_ALT2                (PB1 | ALT2)
#define PB6_ALT1                (PB6 | ALT1)
#endif

#define NUM_DIGITAL_PINS        45
#define NUM_REMAP_PINS          2
#define NUM_ANALOG_INPUTS       16

// I2C definitions
#define PIN_WIRE_SDA            PA10
#define PIN_WIRE_SCL            PA9

// UART Definitions
#define SERIAL_UART_INSTANCE    2

// Default pin used for generic 'Serial' instance
// Mandatory for Firmata
#define PIN_SERIAL_RX           PA3
#define PIN_SERIAL_TX           PA2

// HAL configuration
#define HSE_VALUE               16000000U

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
  // These serial port names are intended to allow libraries and architecture-neutral
  // sketches to automatically default to the correct port name for a particular type
  // of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
  // the first hardware serial port whose RX/TX pins are not dedicated to another use.
  //
  // SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
  //
  // SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
  //
  // SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
  //
  // SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
  //
  // SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
  //                            pins are NOT connected to anything by default.
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR   Serial
  #endif
  #ifndef SERIAL_PORT_HARDWARE
    #define SERIAL_PORT_HARDWARE  Serial
  #endif
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
