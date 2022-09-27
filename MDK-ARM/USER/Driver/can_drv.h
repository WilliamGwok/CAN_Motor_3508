#ifndef __CAN_DRV_H
#define __CAN_DRV_H

#include "stm32f4xx_hal.h"

typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t 			data[8];
} CAN_RxFrameTypeDef;

typedef struct {
	CAN_TxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_TxFrameTypeDef;

void CAN1_Init(void);
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, int16_t *dat);

#endif




