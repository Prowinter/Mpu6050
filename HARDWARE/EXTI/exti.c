#include "exti.h"
#include "delay.h" 
#include "stm32f4xx.h"
#include "usart.h"
#include "inv_mpu.h"
#include "ANO_DT.h"
#define ANO_DT_REPORT     //匿名上位机数据观察，无需观察注释该行。
float pitch,roll,yaw;
//外部中断6服务程序
void EXTI9_5_IRQHandler(void)
{
    extern float pitch,roll,yaw; 		//欧拉角
    static int tem;                //临时变量
    EXTI_ClearITPendingBit(EXTI_Line6); //清除LINE0上的中断标志位 
    if(PBin(6)==0)
    {
        if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
        {
            

            #ifdef ANO_DT_REPORT
            if (tem>10)
            {
                 tem=0;
                 usart1_report_imu((int)(roll*100),(int)(pitch*100),(int)(yaw*10));  //上报欧拉角
            }
            tem++;
            #endif
        }
    }
}
   
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);//PB6 连接到中断线6
    
        /* 配置EXTI_Line6 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;//LINE6
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

  
}
