/*
 * linearTravel_adc.h
 *
 *  Created on: Jul 27, 2021
 *      Author: Calvin
 */

#ifndef INC_LINEARTRAVEL_ADC_H_
#define INC_LINEARTRAVEL_ADC_H_

#include <Timer.h>
#include "window_filtering.h"

#define SUSPENSION_FILTER_SIZE 32

#define NUM_SUSPENSION 2

typedef struct suspension_values {
	uint16_t suspension_value_mapped[NUM_SUSPENSION];

	window_filter_t suspension_value[NUM_SUSPENSION];

	uint16_t raw_suspension_value[NUM_SUSPENSION];
	uint16_t raw_suspension_value_dma[NUM_SUSPENSION];
} suspension_values_t;

extern suspension_values_t current_suspension_values;

extern ms_timer_t timer_suspension_adc;

void setup_suspension_adc();

void suspension_timer_cb(void *args);

#endif /* INC_LINEARTRAVEL_ADC_H_ */
