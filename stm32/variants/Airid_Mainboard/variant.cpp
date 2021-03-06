/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
const PinName digitalPin[] = {
  PA_3,  //D0
  PA_2,  //D1
  PA_10, //D2
  PB_3,  //D3
  PB_5,  //D4
  PB_4,  //D5
  PB_10, //D6
  PA_8,  //D7
  PA_9,  //D8
  PC_7,  //D9
  PB_6,  //D10
  PA_7,  //D11
  PA_6,  //D12
  PA_5,  //D13
  PB_9,  //D14
  PB_8,  //D15
  // ST Morpho
  // CN7 Left Side
  PC_10, //D16
  PC_12, //D17
  NC,    //D18 - BOOT0
  PA_13, //D19 - SWD
  PA_14, //D20 - SWD
  PA_15, //D21
  PB_7,  //D22
  PC_13, //D23
  PC_14, //D24
  PC_15, //D25
  PH_0,  //D26
  PH_1,  //D27
  PC_2,  //D28
  PC_3,  //D29
  // CN7 Right Side
  PC_11, //D30
  PD_2,  //D31
  // CN10 Left Side
  PC_9,  //D32
  // CN10 Right side
  PC_8,  //D33
  PC_6,  //D34
  PC_5,  //D35
  PA_12, //D36
  PA_11, //D37
  PB_12, //D38
  PB_11, //D39
  PB_2,  //D40
  PB_1,  //D41
  PB_15, //D42
  PB_14, //D43
  PB_13, //D44
  PC_4,  //D45
  PA_0,  //D46/A0
  PA_1,  //D47/A1
  PA_4,  //D48/A2
  PB_0,  //D49/A3
  PC_1,  //D50/A4
  PC_0,  //D51/A5
  // Duplicated pins in order to be aligned with PinMap_ADC
  PA_7,  //D52/A6  = D11
  PA_6,  //D53/A7  = D12
  PC_2,  //D54/A8  = D28
  PC_3,  //D55/A9  = D29
  PC_5,  //D56/A10 = D35
  PB_1,  //D57/A11 = D41
  PC_4   //D58/A12 = D45
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 16000000
  *            PLL_M                          = 8
  *            PLL_N                          = 168
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  // Enable Power Control module clock and set voltage scaling to
  // maximum (to allow maximum CPU clock). This uses HAL macros directly
  // rather than the wrapper functions, since the wrapper functions also
  // disable and enable the PLL, which is not what we need here.
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  // Enable HSE Oscillator and activate PLL with HSE as source
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  // Use the HSE as PLL input. The HSE is 16Mhz.
#if HSE_VALUE != 16000000UL
#error "Clock settings assume HSE is 16Mhz"
#endif
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  // PLL input divisor. Must be 1-2Mhz, 2Mhz recommended by F4 reference
  // manual).
  RCC_OscInitStruct.PLL.PLLM = 8;
  // PLL multiplier. Must be between 192Mhz and 432Mhz. Chose to result
  // in 336Mhz, which can be divided to 84Mhz (max system clock) and
  // 48Mhz (exact clock required for USB).
  RCC_OscInitStruct.PLL.PLLN = 168;
  // SYSCLK divider. Divides 336 by 4 to get the max 84Mhz SYSCLK
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  // Peripheral clock divider for USB, ADC and SDIO. Divides 336 by 7 to
  // get the 48Mhz required for USB.
  RCC_OscInitStruct.PLL.PLLQ = 7;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    /* Initialization Error */
    while (1);
  }
  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  // clocks dividers
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  // This divides the SYSCLK to get CPU clock (HCLK). Uses /1 to
  // maximize CPU clock.
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  // This divides HCLK to get the ABP1 bus speed (max 42Mhz).
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  // This divides HCLK to get the ABP2 bus speed (max 84Mhz).
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  // This sets the flash latency to 2 waitstates, which supports up to
  // CPU clock (HCLK) up to 84Mhz at VCC >= 2.7V
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    /* Initialization Error */
    while (1);
  }
}

#ifdef __cplusplus
}
#endif
