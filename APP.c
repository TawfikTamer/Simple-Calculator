/*
 * APP.c
 *
 * Created: 8/24/2023 6:19:03 PM
 * Author : Tawfik
 */ 
#define	F_CPU 16000000UL
#include "LIB/std_types.h"
#include "LIB/common_macros.h"

#include "MCAL/DIO/DIO_interface.h"
#include "HAL/CLCD/CLCD_interface.h"
#include "HAL/KPD/KPD_interface.h"

#include <util/delay.h>
#include <math.h>

u16 ClcArr(u8 NumArr[] , u8 count);

int main(void)
{
   KPD_Init(); 
   CLCD_Init();
   
   u16 num1=0,num2=0,nextnumb = 0;
   
   u8 number1[3];
   u8 number2[3];
   u8 i = 0;
   u8 k = 0;
   u8 opp = 0;
   
    while (1) 
    {
		u8 pressed = KPD_u8GetPressed();

		if(pressed != 0xff)
		{
			if (pressed == '?')
			{
			   num1 = 0;
			   num2 = 0;
			   i = 0;
			   k = 0;
			   opp = 0;
			   nextnumb = 0;
			   CLCD_SendCommand(lcd_Clear);
			}
		
		   if(pressed != '+' && pressed != '-' && pressed != '*' && pressed != '/' && pressed != '=' && pressed != '?' && num1 == 0)
		   {
			   number1[i] = pressed;
			   CLCD_SendNumber(number1[i]);
			   i++;
		   }
		   
		   
		   if(pressed == '+')
		   {
			   num1 = ClcArr(number1,i);
			   CLCD_SendCommand(0x14);
			   CLCD_SendData(pressed);
			   CLCD_SendCommand(0x14);
			   nextnumb = 1;
			   opp = 0;   
		   }
		   
		   if(pressed == '-')
		   {
			   num1 = ClcArr(number1,i);
			   CLCD_SendCommand(0x14);
			   CLCD_SendData(pressed);
			   CLCD_SendCommand(0x14);
			   nextnumb = 1;
			   opp = 1;
		   }
		   
		   if(pressed == '*')
		   {
			   num1 = ClcArr(number1,i);
			   CLCD_SendCommand(0x14);
			   CLCD_SendData(pressed);
			   CLCD_SendCommand(0x14);
			   nextnumb = 1;
			   opp = 2;
			   
		   }
		   
		   if(pressed == '/')
		   {
			   num1 = ClcArr(number1,i);
			   CLCD_SendCommand(0x14);
			   CLCD_SendData(pressed);
			   CLCD_SendCommand(0x14);
			   nextnumb = 1;
			   opp = 3;
			   
		   }
		   
		   
		   
		   if(nextnumb == 1 && pressed != '+' && pressed != '-' && pressed != '*' && pressed != '/' && pressed != '?' && pressed != '=' )
		   {
			  number2[k] = pressed;
			  CLCD_SendNumber(number2[k]);
			  k++;
		   }
		   
		   if (pressed == '=')
		   {
			   num2 = ClcArr(number2,k);
			   CLCD_SetPosition(2,10);
			   CLCD_SendData(pressed);
			   CLCD_SendCommand(lcd_CursorRight);
			   u64 res = 0;
			   f32 dec = 0.0;
			   u8 flt = 0;
			   switch (opp) 
			   {
				   case 0 : 
						res = num1 + num2;
						CLCD_SendNumber(res);
						break;
				   case 1 :
					if(num2 > num1)
					{
						res = num2 - num1;
						CLCD_SendString("-");
					}
					else
					{
						res = num1 - num2;
					}
					    CLCD_SendNumber(res);
						break;
				   case 2 :
						res = num1 * num2;
						CLCD_SendNumber(res);
						break;
				   case 3 :
				       if(num1 % num2 == 0)
					   {
						   res = num1 / num2;
						   CLCD_SendNumber(res);
					   }
					   else if (num2 == 0)
					   {
						   CLCD_SetPosition(2,6);
						   CLCD_SendString("Math ERROR");
					   }
					   
					   else
					   {
							   dec = (f32)num1 / num2 ;
							   res = num1 / num2;
							   flt = (dec - res)*100;
							   CLCD_SendNumber(res);
							   CLCD_SendString(".");
							   CLCD_SendNumber(flt);
					   }
					   
					    
					    break;	
			   }
		   }
		 
		}
			
    }
}


u16 ClcArr(u8 NumArr[] , u8 count)
{
 	u64 result = 0;
	 
	switch (count)
{
	case 1 :
		result = NumArr[0]; 
		break;
	case 2 :
		result = NumArr[0]*10 +NumArr[1];
		break;
	case 3 :
		result = NumArr[0]*100 +NumArr[1]*10 + NumArr[2];
		break;
}

	return result;
}