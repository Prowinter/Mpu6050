#include "ANO_DT.h"
#include "usart.h"
// 此代码为下位机代码，与匿名科创上位机V65协同

void TestSendData(u8 *dataToSend , u8 length)
{
	u8 t;
	for(t=0;t<length;t++)							
	{
		USART_SendData(USART1,*dataToSend);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
		dataToSend++;
	}
}
//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,c);   

} 

//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32]={0};
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0XAA;	//帧头
	send_buf[1]=0X05;	//发送设备
	send_buf[2]=0XAF;	//目标设备
	send_buf[3]=fun;	//功能字
	send_buf[4]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[5+i]=data[i];			//复制数据
	for(i=0;i<len+5;i++)send_buf[len+5]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+6;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[18]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	tbuf[12]=0;
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	tbuf[16]=0;
	tbuf[17]=0;
	usart1_niming_report(0X02,tbuf,18);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short roll,short pitch,short yaw)
{
	u8 tbuf[13]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF;
	tbuf[6]=(0>>8)&0XFF;
	tbuf[7]=0&0XFF;
	tbuf[8]=(0>>8)&0XFF;
	tbuf[9]=0&0XFF;
	tbuf[10]=0&0XFF;
	tbuf[11]=0x01&0XFF;
	tbuf[12]=0&0XFF;

	usart1_niming_report(0X01,tbuf,13);//飞控显示帧,0XAF
} 

//PID调试专用
void pid_debug(short wheel_1,short wheel_2,short wheel_3,short wheel_4,short Speed_Set,short pwm_1,short pwm_2,short pwm_3,short pwm_4,u8 kp,u8 ki,u8 kd)
{
	u8 tbuf[21]={0}; 
	tbuf[0]=(wheel_1>>8)&0XFF;
	tbuf[1]=wheel_1&0XFF;
	tbuf[2]=(wheel_2>>8)&0XFF;
	tbuf[3]=wheel_2&0XFF;
	tbuf[4]=(wheel_3>>8)&0XFF;
	tbuf[5]=wheel_3&0XFF;
	tbuf[6]=(wheel_4>>8)&0XFF;
	tbuf[7]=wheel_4&0XFF;
    tbuf[8]=(Speed_Set>>8)&0XFF;
	tbuf[9]=Speed_Set&0XFF;
    tbuf[10]=(pwm_1>>8)&0XFF;
	tbuf[11]=pwm_1&0XFF;
    tbuf[12]=(pwm_2>>8)&0XFF;
	tbuf[13]=pwm_2&0XFF;
    tbuf[14]=(pwm_3>>8)&0XFF;
	tbuf[15]=pwm_3&0XFF;
    tbuf[16]=(pwm_4>>8)&0XFF;
	tbuf[17]=pwm_4&0XFF;
	tbuf[18]=kp&0XFF;
	tbuf[19]=ki&0XFF;
	tbuf[20]=kd&0XFF;
	usart1_niming_report(0XF1,tbuf,21);//飞控显示帧,0XAF
}



