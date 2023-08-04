#include "GPIO.h"

GPIO_InitTypeDef GPIO_InitStructure;



/*----------------------------------------------------------------------------
  SET BIT OUTPUT
 *----------------------------------------------------------------------------
void IO_BIT_SET (GPIO_TypeDef *GPIOx,uint32_t num) {

  GPIOx->BSRR = (1UL << num);                 // Turn On  I/O                
}
*/

/*----------------------------------------------------------------------------
  RESET BIT OUTPUT
 *---------------------------------------------------------------------------
void IO_BIT_RESET (GPIO_TypeDef *GPIOx,uint32_t num) {

  GPIOx->BRR  = (1UL << num);                 // Turn Off I/O                
}
*/

void GPIO_OUT (GPIO_TypeDef *GPIOx,uint32_t num,uint32_t val) {
	/*
	uint32_t temp=GPIOx->ODR & ((1UL << num));			//read pin value
	if(temp!=0 && val==0)
	{
		GPIOx->ODR  &=~((1UL << num));                 
	}
	else if(temp==0 && val==1)
	{
		GPIOx->ODR  |=((1UL << num));
	}
	*/
//uint32_t temp=GPIOx->ODR;	
//temp &= ~((1UL << num));	
//temp |= val & ((1UL << num));	
//GPIOx->ODR  =temp;

	if((val&1UL)==0)
	{
		GPIOx->BRR=(1UL << num);
	}
	else
	{
		GPIOx->BSRR=(1UL << num);
	}
	
}

void GPIO_Configuration(void)
{		
		GPIO_PinConfigure(BT_LED_PORT,BT_LED,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT50MHZ);
		GPIO_PinConfigure(CPU_LED_PORT,CPU_LED,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT50MHZ);
		
	
		GPIO_OUT(BT_LED_PORT,BT_LED,BT_LED_INIT);
		GPIO_OUT(CPU_LED_PORT,CPU_LED,CPU_LED_INIT);
	
}

/*
void Enable_MCO_Pin(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    //Output clock on MCO pin --------------------------------------------- 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    //RCC_MCOConfig(RCC_MCO_HSE); // Put on MCO pin the: freq. of external crystal 
    RCC_MCOConfig(RCC_MCO_SYSCLK);  // Put on MCO pin the: System clock selected  
}
*/

