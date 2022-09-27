#include "can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Motor.h"

#define MotorStdID  0x200
extern CAN_HandleTypeDef hcan1;
int16_t SendBuff[4];//�������ݰ����ҵĵ��IDΪ2����data[1]���͵�������Ч

 void Start_CAN1_Task(void const * argument)
{
  /* USER CODE BEGIN Start_CAN1_Task */
	CAN1_Init();
	//��䷢�����ݰ�
	SendBuff[1]=700;
  /* Infinite loop */
  for(;;)
  {
		SendBuff[1]=700;
		CAN_SendData(&hcan1,MotorStdID,&SendBuff[0]);
    osDelay(100);
		//������ݰ�
		SendBuff[1]=0;
  }
  /* USER CODE END Start_CAN1_Task */
}

