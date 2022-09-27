#include "can_drv.h"
#include "Motor.h"

extern CAN_HandleTypeDef hcan1;
CAN_RxFrameTypeDef hcan1RxFrame;//接收数据帧结构体，有header和 uint8_t data[8]
CAN_TxFrameTypeDef hcan1TxFrame;//发送数据帧结构体，有header和 uint8_t data[8]
uint16_t lasttime;//这三个变量用于测量电机发送报文的周期
uint16_t thistime;
uint16_t err;
/**
 *	@brief	CAN标识符过滤器复位为默认
 */
static void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig)
{
	sFilterConfig->FilterIdHigh = 0;						
	sFilterConfig->FilterIdLow = 0;							
	sFilterConfig->FilterMaskIdHigh = 0;					
	sFilterConfig->FilterMaskIdLow = 0;						
	sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0;	
	sFilterConfig->FilterBank = 0;							
	sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;		
	sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;		
	sFilterConfig->FilterActivation = ENABLE;				
	sFilterConfig->SlaveStartFilterBank = 0;
}

/**
 *	@brief	CAN1 接收中断回调函数
 */
static void CAN_Rx_Callback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		
		//在这里更新电机的数据
		MotorUpdate(hcan1RxFrame.data);
	}
}

/**
 *	@brief	重写CAN RxFIFO中断接收函数
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	thistime=HAL_GetTick();
	err=thistime-lasttime;
	lasttime=thistime;
	if(hcan->Instance == CAN1)
	{
		CAN_Rx_Callback(hcan);
	}
}

uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, int16_t *dat)
{
	uint32_t txMailBox;
	CAN_TxFrameTypeDef *txFrame;
	if(hcan->Instance == CAN1)
		txFrame = &hcan1TxFrame;
	//配置报文信息
	txFrame->header.StdId = stdId;
	txFrame->header.IDE = CAN_ID_STD;
	txFrame->header.RTR = CAN_RTR_DATA;
	txFrame->header.DLC = 8;
	//先发高8位，再发低8位，需要传入一个4个int16的数组
	txFrame->data[0] = (uint8_t)((int16_t)dat[0] >> 8);
	txFrame->data[1] = (uint8_t)((int16_t)dat[0]);
	txFrame->data[2] = (uint8_t)((int16_t)dat[1] >> 8);
	txFrame->data[3] = (uint8_t)((int16_t)dat[1]);
	txFrame->data[4] = (uint8_t)((int16_t)dat[2] >> 8);
	txFrame->data[5] = (uint8_t)((int16_t)dat[2]);
	txFrame->data[6] = (uint8_t)((int16_t)dat[3] >> 8);
	txFrame->data[7] = (uint8_t)((int16_t)dat[3]);
	//发送数据
	HAL_CAN_AddTxMessage(hcan, &txFrame->header, &txFrame->data[0], &txMailBox);
	return HAL_OK;
}

/**
 *	@brief	CAN1 初始化
 */
void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	
	HAL_CAN_Start(&hcan1);
}
