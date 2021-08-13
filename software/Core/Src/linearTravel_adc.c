/*
 * linearTravel_adc.c
 *
 *  Created on: Jul 27, 2021
 *      Author: Calvin
 */

#include "main.h"
#include "linearTravel_adc.h"
#include "adc.h"
#include "can.h"
#include <General_Sensor_CAN_Messages.h>

#include <stdio.h>

suspension_values_t current_suspension_values;
ms_timer_t timer_suspension_adc;

int count = 0;

bool suspension_update = false;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	current_suspension_values.raw_suspension_value[0] = current_suspension_values.raw_suspension_value_dma[0];
	current_suspension_values.raw_suspension_value[1] = current_suspension_values.raw_suspension_value_dma[1];
}

void setup_suspension_adc() {
	// every 10ms, continuous
	timer_suspension_adc = timer_init(10, true, suspension_timer_cb);

	// start dma requests
	HAL_ADC_Start_DMA(&hadc, current_suspension_values.raw_suspension_value_dma,NUM_SUSPENSION);

	// wait for first dma round
	HAL_Delay(1);

	// setup filters
	for (int i = 0; i < NUM_SUSPENSION; i++) {
		window_filter_initialize(&current_suspension_values.suspension_value[i],
				(uint16_t) current_suspension_values.raw_suspension_value[i],
				SUSPENSION_FILTER_SIZE);
	}

	// start timer
	timer_start(&timer_suspension_adc);
}

void suspension_timer_cb(void *args) {
	HAL_ADC_Start_DMA(&hadc, current_suspension_values.raw_suspension_value_dma,NUM_SUSPENSION);

	// update filter
	for (int i = 0; i < NUM_SUSPENSION; i++) {
		window_filter_update(&current_suspension_values.suspension_value[i],
				current_suspension_values.raw_suspension_value[i]);
	}

	count++;

	if (count >= 10) {
		count = 0;

		// log to CAN
		General_Sensor_LinearPot_t msg = Compose_General_Sensor_Travel(
				0,
				current_suspension_values.suspension_value[0].current_filtered,
				current_suspension_values.suspension_value[1].current_filtered);

		printf("A %d %d\r\n", current_suspension_values.suspension_value[0].current_filtered,
				current_suspension_values.suspension_value[1].current_filtered);

		CAN_TxHeaderTypeDef header = { .ExtId = msg.id,
				.IDE = CAN_ID_EXT,
				.RTR = CAN_RTR_DATA,
				.DLC = sizeof(msg.data),
				.TransmitGlobalTime = DISABLE };
		HAL_CAN_AddTxMessage(&hcan, &header, msg.data,
				&can_mailbox);


	}
}
