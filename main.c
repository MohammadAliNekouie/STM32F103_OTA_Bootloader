
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "flash.h"
#include "GPIO.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
typedef void(*pFunction)(void);
void jump_to_start(void);
/* Jump Parameters */
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint16_t  app_main_checksum=0;
uint16_t  app_new_checksum=0;




int main(void)
{
	//Enable_MCO_Pin();				/*  Config MCO pin as Output for System Clock  */
	GPIO_Configuration(); 	/*  Define Input/Output pins and initial values */
	GPIO_OUT(CPU_LED_PORT,CPU_LED,1);
	GPIO_OUT(BT_LED_PORT,BT_LED,1);
	
	while(1)
	{		
		// each firmware on the Flash has a CRC that calculated in programming
		// also has a ONE BYTE signature for validity checking
		// if CRC are different and signeture byte = 63 then new firmware detected!
		app_main_checksum=(FEE_ReadDataByte_abs(Flash_lower_half_end-2-255)<<8)|FEE_ReadDataByte_abs(Flash_lower_half_end-1-255);
		app_new_checksum=(FEE_ReadDataByte_abs(Flash_uppper_half_end-2-255)<<8)|FEE_ReadDataByte_abs(Flash_uppper_half_end-1-255);
		
		if(app_main_checksum!=app_new_checksum)
	  {
			if(FEE_ReadDataByte_abs(Flash_uppper_half_end-4-255)==0x3F)
			{
				//GPIO_OUT(CPU_LED_PORT,CPU_LED,1);
				//copy new firmware to lower flash location
				FEE_Init();
				FEE_Erase();
				Page_Copy(Flash_lower_half_start,Flash_uppper_half_start,Flash_uppper_half_end);
				FLASH_LockBank1();
			}
		}
		
		jump_to_start();
		while(1)
		{
			for(JumpAddress=0;JumpAddress<0x000FFFFF;JumpAddress++)
			{
			}
			GPIO_OUT(BT_LED_PORT,BT_LED,~GPIO_PinRead(BT_LED_PORT,BT_LED));
			GPIO_OUT(CPU_LED_PORT,CPU_LED,~GPIO_PinRead(CPU_LED_PORT,CPU_LED));
		}
	}
}





void jump_to_start(void)
{
	
	/* If Program has been written */
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
		{
			
			//Make sure, the CPU is in privileged mode.
			//if( CONTROL_nPRIV_Msk & __get_CONTROL( ) )
			//{  /* not in privileged mode */
			//	EnablePrivilegedMode();
			//}			
			//Disable all enabled interrupts in NVIC.
			
			NVIC->ICER[ 0 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 1 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 2 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 3 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 4 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 5 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 6 ] = 0xFFFFFFFF ;
			NVIC->ICER[ 7 ] = 0xFFFFFFFF ;
			
			//Clear all pending interrupt requests in NVIC.
			
			NVIC->ICPR[ 0 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 1 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 2 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 3 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 4 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 5 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 6 ] = 0xFFFFFFFF ;
			NVIC->ICPR[ 7 ] = 0xFFFFFFFF ;
			
			//Disable SysTick and clear its exception pending bit,
			
			//__disable_irq();//EEEEERRRRRRROOOORRRR
			//__DSB();//EEEEERRRRRRROOOORRRR
			//__ISB();//EEEEERRRRRRROOOORRRR
		 
			SysTick->CTRL=0;		
			SysTick->LOAD=0;
			SysTick->VAL=0;
			///////////////////////__set_PRIMASK(1);/////////////////////////////EEEEERRRRRRROOOORRRR
			
			SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
			
			//Disable individual fault handlers if the bootloader used them.
			SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk );
			//Activate the MSP, if the core is found to currently run with the PSP.
			if(CONTROL_SPSEL_Msk& __get_CONTROL())
			{  /* MSP is not active */
				__set_CONTROL( __get_CONTROL( ) & ~CONTROL_SPSEL_Msk );
			}
			
			//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);
			
			/* Set system control register SCR->VTOR  */
			//SCB->VTOR = NVIC_VectTab_FLASH | ((ApplicationAddress & 0x0000FFFF) & (uint32_t)0x1FFFFF80);
			
			NVIC_SetVectorTable(NVIC_VectTab_FLASH, ApplicationAddress & 0x0000FFFF);			
			
			//__DSB();
			//__ISB();
			
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			Jump_To_Application = (void (*)(void)) (*((uint32_t *) ((ApplicationAddress + 4))));
			Jump_To_Application(); 
		}
}

