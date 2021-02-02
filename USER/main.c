#include "delay.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "exti.h"
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
    EXTIX_Init();
	MPU_Init();					//��ʼ��MPU6050
	while(mpu_dmp_init());
 	while(1)
	{
//        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
        delay_ms(200);
	} 	
}
