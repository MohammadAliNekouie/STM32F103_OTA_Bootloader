//-----------------------------------------------------------------------------
// Copyright:  TESLA ELECTRONIC COMPANY    
// Author:  Mohammad Ali Nekouie       
// Remarks:  M.A.N.      
// known Problems: !!!
// Version:  0.1     
// Description:  This Lib create for GPIO TCA V2.0 definition in STM32F103 
//-----------------------------------------------------------------------------

#include <stm32f10x.h>                       /* STM32F10x definitions         */
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////      I/O      ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
/*----------------------------------------------------------------------------
                              Declare OUTPUT Pins
*----------------------------------------------------------------------------*/
#define BT_LED 								5
#define BT_LED_PORT 					GPIOC
#define BT_LED_INIT						0

#define CPU_LED 							4
#define CPU_LED_PORT 					GPIOC
#define	CPU_LED_INIT					0

void GPIO_Configuration(void);
//void Enable_MCO_Pin(void);
//void IO_BIT_SET (GPIO_TypeDef *GPIOx,uint32_t num);
//void IO_BIT_RESET (GPIO_TypeDef *GPIOx,uint32_t num);
void GPIO_OUT (GPIO_TypeDef *GPIOx,uint32_t num,uint32_t val);

