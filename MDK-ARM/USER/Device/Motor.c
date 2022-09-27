#include "Motor.h"

typedef struct motor_info_struct {
	uint16_t	  angle;
	int16_t		speed;
	int16_t		current;
	uint8_t   temperature;
//	int16_t   Torque;
} motor_info_t;

motor_info_t *motor_info;

/**
 *	@brief	��ȡ����Ƕ�λ��
 */
static uint16_t CAN_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = ((uint16_t)rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	��ȡ���ת��ת��
 */
static int16_t CAN_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = ((uint16_t)rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	��ȡ���ʵ��ת�ص���
 */
static int16_t CAN_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = ((int16_t)rxData[4] << 8 | rxData[5]);
	return current;
}

/**
 *	@brief	��ȡ���ʵ���¶�
 */
static uint8_t CAN_GetMotorTemperature(uint8_t *rxData)
{
	uint8_t temperature;
	temperature = rxData[6];
	return temperature;
}

/**
 *	@brief	���µ������
 */
void MotorUpdate(uint8_t *rxBuf)
{
	
	motor_info->angle   = CAN_GetMotorAngle(rxBuf);	
	
	motor_info->speed   = CAN_GetMotorSpeed(rxBuf);
	
	motor_info->current = CAN_GetMotorCurrent(rxBuf);
	
	motor_info->temperature = CAN_GetMotorTemperature(rxBuf);
}

