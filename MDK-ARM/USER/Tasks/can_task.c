#include "can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Motor.h"

#define MotorStdID  0x200
extern CAN_HandleTypeDef hcan1;
int16_t SendBuff[4];//发送数据包，我的电机ID为2，故data[1]发送的数据有效

 void Start_CAN1_Task(void const * argument)
{
  /* USER CODE BEGIN Start_CAN1_Task */
	CAN1_Init();
	//填充发送数据包
	SendBuff[1]=700;
  /* Infinite loop */
  for(;;)
  {
		SendBuff[1]=700;
		CAN_SendData(&hcan1,MotorStdID,&SendBuff[0]);
    osDelay(100);
		//清空数据包
		SendBuff[1]=0;
  }
  /* USER CODE END Start_CAN1_Task */
}

