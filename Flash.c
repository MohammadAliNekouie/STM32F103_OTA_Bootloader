#include <stdio.h>
#include <string.h>
#include "stm32f10x_flash.h"
#include "flash.h"
//#include "GPIO.h"
/*****************************************************************************
 * Allows to use the internal flash to store non volatile data. To initialize
 * the functionality use the FEE_Init() function. Be sure that by reprogramming
 * of the controller just affected pages will be deleted. In other case the non
 * volatile data will be lost.
******************************************************************************/
//remember for flash , you just can write one time after erase and for next write 
//you must erase whole of flash page.
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

uint8_t DataBuf[FEE_PAGE_SIZE];
/*****************************************************************************
*  Delete Flash Space used for user Data, deletes the whole space between
*  RW_PAGE_BASE_ADDRESS and the last uC Flash Page
******************************************************************************/
uint16_t
FEE_Init(void) {
	// unlock flash
	FLASH_Unlock();

	// Clear Flags
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);

	return FEE_DENSITY_BYTES;
}
/*****************************************************************************
*  Erase the whole reserved Flash Space used for user Data
******************************************************************************/
void
FEE_Erase (void) {

	int page_num = 0;

	// delete all pages from specified start page to the last page
	do {
		FLASH_ErasePage(Flash_lower_half_start + (page_num * FEE_PAGE_SIZE));
		page_num++;
	} while ((Flash_lower_half_start + (page_num * FEE_PAGE_SIZE)) < Flash_lower_half_end);
}
/*****************************************************************************
*  Writes once data byte to flash on specified address. If a byte is already
*  written, the whole page must be copied to a buffer, the byte changed and
*  the manipulated buffer written after PageErase.
*******************************************************************************/
uint16_t
FEE_WriteMultiple(uint32_t Address,uint8_t * Value,uint16_t len) {

	FLASH_Status FlashStatus = FLASH_COMPLETE;
	int i;

		// Write new data (whole page) to flash if data has beed changed
		for(i = 0; i < len; i=i+2) 
		{
				FlashStatus = FLASH_ProgramHalfWord(Address + i , (uint16_t)((DataBuf[i+1]<<8) | DataBuf[i]));
				//GPIO_OUT(CPU_LED_PORT,CPU_LED,~GPIO_PinRead(CPU_LED_PORT,CPU_LED));
		}
	
	return FlashStatus;
}

// Load all of page from start to desired length into a buffer pointer
// change specificed variable in a address in that buffer
// Erase whole of page and write buffer on the Flash page.
/*
void FEE_LoadPageThenChange(char * Buffer,uint16_t Buffer_length,uint16_t var_Address,char * Value,uint16_t var_Lenght)
{
	uint16_t temp;
	FEE_ReadMultiple(0,Buffer,Buffer_length);
	for(temp=0;temp<var_Lenght;temp++)
	{
		Buffer[var_Address+temp]=*(Value+temp);
	}
	FEE_Erase();
	FEE_WriteMultiple(0,Buffer,Buffer_length);
}
*/


//all pages must erased before run this function and Flash must be unlocked
void Page_Copy(uint32_t dest_Page_start_address,uint32_t source_Page_start_address,uint32_t source_Page_end_address)
{
	uint16_t page_num=((source_Page_end_address-source_Page_start_address)+1)/FEE_PAGE_SIZE;
	uint32_t read_address=source_Page_start_address;
	uint32_t write_address=dest_Page_start_address;
	uint16_t i=0;
	
	for(i=0;i<page_num;i++)
	{
		FEE_ReadMultiple(read_address,DataBuf,FEE_PAGE_SIZE);
		FEE_WriteMultiple(write_address,DataBuf,FEE_PAGE_SIZE);
		write_address+=FEE_PAGE_SIZE;		
	}
}

/*****************************************************************************
*  Read once data byte from a specified address.
*******************************************************************************/
uint8_t
FEE_ReadDataByte (uint32_t Address) {

	uint8_t DataByte = 0xFF;

	// Get Byte from specified address
	DataByte = (*(uint8_t*)(Address));

	return DataByte;
}

uint8_t
FEE_ReadDataByte_abs (uint32_t BASE_ADDRESS) {

	uint8_t DataByte = 0xFF;

	// Get Byte from specified address
	DataByte = (*(uint8_t*)(BASE_ADDRESS));

	return DataByte;
}

void FEE_ReadMultiple (uint32_t Address,uint8_t * Dest,uint16_t len)
{
	uint16_t temp;
	uint8_t result;
	for(temp=0;temp<len;temp++)
	{
		result=FEE_ReadDataByte(Address+temp);
		*(Dest+temp)=result;
	}
	//*(Dest+len)=0;	
}


