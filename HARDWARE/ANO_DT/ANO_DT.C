#include "ANO_DT.h"
#include "usart.h"
// �˴���Ϊ��λ�����룬�������ƴ���λ��V65Эͬ

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
//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,c);   

} 

//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32]={0};
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0XAA;	//֡ͷ
	send_buf[1]=0X05;	//�����豸
	send_buf[2]=0XAF;	//Ŀ���豸
	send_buf[3]=fun;	//������
	send_buf[4]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[5+i]=data[i];			//��������
	for(i=0;i<len+5;i++)send_buf[len+5]+=send_buf[i];	//����У���	
	for(i=0;i<len+6;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
	usart1_niming_report(0X02,tbuf,18);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short roll,short pitch,short yaw)
{
	u8 tbuf[13]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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

	usart1_niming_report(0X01,tbuf,13);//�ɿ���ʾ֡,0XAF
} 

//PID����ר��
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
	usart1_niming_report(0XF1,tbuf,21);//�ɿ���ʾ֡,0XAF
}


