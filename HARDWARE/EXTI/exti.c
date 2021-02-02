#include "exti.h"
#include "delay.h" 
#include "stm32f4xx.h"
#include "usart.h"
#include "inv_mpu.h"
#include "ANO_DT.h"
#define ANO_DT_REPORT     //������λ�����ݹ۲죬����۲�ע�͸��С�
float pitch,roll,yaw;
//�ⲿ�ж�6�������
void EXTI9_5_IRQHandler(void)
{
    extern float pitch,roll,yaw; 		//ŷ����
    static int tem;                //��ʱ����
    EXTI_ClearITPendingBit(EXTI_Line6); //���LINE0�ϵ��жϱ�־λ 
    if(PBin(6)==0)
    {
        if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
        {
            

            #ifdef ANO_DT_REPORT
            if (tem>10)
            {
                 tem=0;
                 usart1_report_imu((int)(roll*100),(int)(pitch*100),(int)(yaw*10));  //�ϱ�ŷ����
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);//PB6 ���ӵ��ж���6
    
        /* ����EXTI_Line6 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;//LINE6
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

  
}
