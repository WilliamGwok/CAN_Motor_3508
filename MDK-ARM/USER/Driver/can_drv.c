#include "can_drv.h"
#include "Motor.h"

extern CAN_HandleTypeDef hcan1;
CAN_RxFrameTypeDef hcan1RxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
CAN_TxFrameTypeDef hcan1TxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
uint16_t lasttime;//�������������ڲ���������ͱ��ĵ�����
uint16_t thistime;
uint16_t err;
/**
 *	@brief	CAN��ʶ����������λΪĬ��
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
 *	@brief	CAN1 �����жϻص�����
 */
static void CAN_Rx_Callback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		
		//��������µ��������
		MotorUpdate(hcan1RxFrame.data);
	}
}

/**
 *	@brief	��дCAN RxFIFO�жϽ��պ���
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
	//���ñ�����Ϣ
	txFrame->header.StdId = stdId;
	txFrame->header.IDE = CAN_ID_STD;
	txFrame->header.RTR = CAN_RTR_DATA;
	txFrame->header.DLC = 8;
	//�ȷ���8λ���ٷ���8λ����Ҫ����һ��4��int16������
	txFrame->data[0] = (uint8_t)((int16_t)dat[0] >> 8);
	txFrame->data[1] = (uint8_t)((int16_t)dat[0]);
	txFrame->data[2] = (uint8_t)((int16_t)dat[1] >> 8);
	txFrame->data[3] = (uint8_t)((int16_t)dat[1]);
	txFrame->data[4] = (uint8_t)((int16_t)dat[2] >> 8);
	txFrame->data[5] = (uint8_t)((int16_t)dat[2]);
	txFrame->data[6] = (uint8_t)((int16_t)dat[3] >> 8);
	txFrame->data[7] = (uint8_t)((int16_t)dat[3]);
	//��������
	HAL_CAN_AddTxMessage(hcan, &txFrame->header, &txFrame->data[0], &txMailBox);
	return HAL_OK;
}

/**
 *	@brief	CAN1 ��ʼ��
 */
void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	
	HAL_CAN_Start(&hcan1);
}
