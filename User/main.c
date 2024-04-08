#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyUSART.H"
#include "Delay.h"
#include "esp.h"
#include <stdio.h>
#include "PWM.h"
uint8_t State;

extern char RECS[200];
extern char Data[5];

uint16_t cnt;

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//	
//	
	uint8_t Judge=0;
	MyUSART_Init();
	
	OLED_Init();
	OLED_ShowString(1,1,"Linking...");

	//
	do
	{
		Judge = esp_Init();
		OLED_ShowString(1,1,"error code:    ");
		OLED_ShowNum(2,1,Judge,1);
	}while(Judge);  //连接阿里云直到成功
	
	OLED_ShowString(3,1,"Lock    Switch");
	OLED_ShowString(1,1,"State  Humidi");

	while(1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(100);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(100);
		
		cnt++;
		if(cnt%150==0)
//			DHT11_GetData(); //约每1s执行一次温湿度采集
		if(cnt==600) //约每6s执行一次数据上报
		{
			if(Esp_PUB() == 1)
			{
				OLED_ShowString(1,1,"publish failed");
				Delay_ms(500);
				OLED_Clear();
			}
			cnt=0;
		}
		
//		Shidu=Data[0];
//		Temperature=Data[2];
//		OLED_ShowNum(2,2,State,2);
		OLED_ShowChar(2,2,State);
		switch(State)
			{
			case 0:
				
			break;
			case 1:
				
			case 2:
				
			break;
			case 3:
				
			break;
		}
		
//		if(Lock=='0')
//			PWM_SetCompare(5);
//		else if(Lock=='1')
//			PWM_SetCompare(15);
//		if(Switch2=='0')
//			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//		else if(Switch2=='1')
//			GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}
