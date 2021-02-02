#ifndef _DATA_TRANSFER_H
#define	_DATA_TRANSFER_H

#include "stm32f4xx.h"
#include "usart.h"
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
void TestSendData(u8 *dataToSend , u8 length);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void usart1_send_char(u8 c);
void pid_debug(short wheel_1,short wheel_2,short wheel_3,short wheel_4,short Speed_Set,short pwm_1,short pwm_2,short pwm_3,short pwm_4,u8 kp,u8 ki,u8 kd);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short roll,short pitch,short yaw);
#endif

