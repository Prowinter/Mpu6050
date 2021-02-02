#include "delay.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "exti.h"
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
    EXTIX_Init();
	MPU_Init();					//初始化MPU6050
	while(mpu_dmp_init());
 	while(1)
	{
//        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
        delay_ms(200);
	} 	
}
