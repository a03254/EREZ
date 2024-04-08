#include "stm32f10x.h"                  // Device header
#include "MyUSART.h"
#include <stdio.h>
#include <string.h>
#include "Delay.h"
#include "OLED.H"
#include "esp.h"

extern uint8_t State;//变量


extern char RECS[250];//串口缓冲量
const char* WIFI ="Cool";//wifi名
const char* WIFIASSWORD="00000001";//wifi密码

const char* ClintID="k0zqkuJFXzo.mycar|securemode=2\\,signmethod=hmacsha256\\,timestamp=1711440538803|";//请见阿里云中设备信息的MQTT连接参数
const char* username="mycar&k0zqkuJFXzo";
const char* passwd="fb2c1693f2f45c667551c5383861559b92407188145a9c6d150693131f079f9b";
const char* Url="iot-06z00fct04kcw3n.mqtt.iothub.aliyuncs.com";
//产品中的物理型通信Topic
const char* pubtopic="/sys/k0zqkuJFXzo/mycar/thing/event/property/post";//设备属性上报

const char* subtopic="/sys/k0zqkuJFXzo/mycar/thing/event/property/post_reply";//云端响应属性上报

const char* func1="State";//所定义的功能的标识符
//const char* func2="humidity"; //举例
//const char* func3="powerstate_1";
//const char* func4="powerstate_2";
int fputc(int ch,FILE *f )   //printf重定向  使用的时候要打开MicroLib否则烧录完是个无意义的函数无法执行
{
	USART_SendData(USART1,(uint8_t)ch);
	while(USART_GetFlagStatus (USART1,USART_FLAG_TC) == RESET);
	return ch;
}
char esp_Init(void)
{
	memset(RECS,0,sizeof(RECS));
	printf("AT+RST\r\n");  //重启
	Delay_ms(2000);
	
	memset(RECS,0,sizeof(RECS));
	printf("ATE0\r\n");    //关闭回显
	Delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 1;
	
	printf("AT+CWMODE=1\r\n"); //Station模式
	Delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 2;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD); //连接热点
	Delay_ms(2000);
	if(strcmp(RECS,"OK"))
		return 3;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClintID,username,passwd);//用户信息配置
	Delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 4;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url); //连接服务器
	Delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 5;
	
	printf("AT+MQTTSUB=0,\"%s\",1\r\n",subtopic); //订阅消息
	Delay_ms(500);
	if(strcmp(RECS,"OK"))
		return 5;
	memset(RECS,0,sizeof(RECS));
	return 0;
}
//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
char Esp_PUB(void)
{
	memset(RECS,0,sizeof(RECS));//下面这行要对照着json的格式去填入变量（上文已经宏定义）及其类型
//printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d}}\",0,0\r\n",pubtopic,func1,Temperature,func2,Shidu,func3,Lock,func4,Switch2);
	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d}}\",0,0\r\n",pubtopic,func1,State);

	//while(RECS[0]);//等待ESP返回数据
	Delay_ms(200);//延时等待数据接收完成
	if(strcmp(RECS,"ERROR")==0)
		return 1;
	return 0;
}

void CommandAnalyse(void)//扫描以及分析函数，将回传的mqtt消息进行分析
{
	if(strncmp(RECS,"+MQTTSUBRECV:",13)==0)//表示这段字符串的长度
	{
		uint8_t i=0;
		while(RECS[i++] != '\0')             
		{
			if(strncmp((RECS+i),func1,5)==0)//这里的5表示State的长度
			{
				while(RECS[i++] != ':');       
				State=RECS[i];//通过对扫描获取的数据传回主函数，使用方法请见下面注释掉的函数
			}
			
		}
	}
}


//void CommandAnalyse(void)
//{
//	if(strncmp(RECS,"+MQTTSUBRECV:",13)==0)
//	{
//		uint8_t i=0;
//		while(RECS[i++] != '\0')             
//		{
//			if(strncmp((RECS+i),func3,12)==0)
//			{
//				while(RECS[i++] != ':');       
//				Lock=RECS[i];
//			}
//			if(strncmp((RECS+i),func4,12)==0)
//			{
//				while(RECS[i++] != ':');
//				Switch2=RECS[i];
//			}
//		}
//	}
//}









