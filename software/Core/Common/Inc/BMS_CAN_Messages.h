/**
 ******************************************************************************
 * @file BMS_CAN_Messages.h
 * @brief BMS CAN Messages
 ******************************************************************************
 */

#ifndef INC_BMS_CAN_MESSAGES_H_
#define INC_BMS_CAN_MESSAGES_H_

#include "stdbool.h"
#include "main.h"
#include "can.h"
#include <stdlib.h>
#include "QUTMS_can.h"

/**
 * @brief BMS Bad Cell Voltage Message
 */
typedef struct BMS_BadCellVoltage
{
	uint32_t id; /**< CAN Packet ID*/
	uint8_t data[2]; /**< Data */
} BMS_BadCellVoltage_t;

/**
 * @brief BMS Bad Cell Voltage Message Composer
 * @param BMSId The BMS ID
 * @param cellNumber The cell number relating to the voltage
 * @param voltage The voltage of said cell
 * @return The generated BMS_BadCellVoltage_t packet
 */
BMS_BadCellVoltage_t Compose_BMS_BadCellVoltage(uint8_t BMSId, uint8_t cellNumber, uint8_t voltage);

/**
 * @brief BMS Bad Cell Voltage Message Parser
 * @param packet The BMS_BadCellVoltage_t packet to parse
 * @param BMSId The BMS ID
 * @param cellNumber The cell number relating to the voltage
 * @param voltage The voltage of said cell
 */
void Parse_BMS_BadCellVoltage(uint8_t* data, uint8_t* BMSId, uint8_t* cellNumber, uint8_t* voltage);

/**
 * @brief BMS Bad Cell Temperature Message
 */
typedef struct BMS_BadCellTemperature
{
	uint32_t id; /**< CAN Packet ID */
	uint8_t data[2]; /**< Data */
} BMS_BadCellTemperature_t;

/**
 * @brief BMS Bad Cell Temperature Message Composer
 * @param BMSId The BMS ID
 * @param cellNumber The Cell number relating to the temperature
 * @param temperature The temperature of said cell
 * @return The composed BadCellTemperature_t packet
 */
BMS_BadCellTemperature_t Compose_BMS_BadCellTemperature(uint8_t BMSId, uint8_t cellNumber, uint8_t temperature);

/**
 * @brief BMD Bad Cell Temperature Message Parser
 * @param packet The BMS_BadCellTemperature_t packet to parse
 * @param BMSId The BMS ID
 * @param cellNumber The Cell number relating to the temperature
 * @param temperature The temperature of said cell
 */
void Parse_BMS_BadCellTemperature(uint8_t* data, uint8_t* BMSId, uint8_t* cellNumber, uint8_t* temperature);

/**
 * @brief BMS Transmit Voltage Message
 */
typedef struct BMS_TransmitVoltage
{
	uint32_t id; /**< CAN Packet ID */
	uint8_t data[8]; /**< Data */
} BMS_TransmitVoltage_t;

/**
 * @brief BMS Transmit Voltage Message Composer
 * @param BMSId The BMS ID
 * @param vMsgId The voltage message ID (0 or 1) as we send 2 packets with all voltages
 * @param voltages The voltages to send
 * @return The composed BMS_TransmitVoltage_t packet
 */
BMS_TransmitVoltage_t Compose_BMS_TransmitVoltage(uint8_t BMSId, uint8_t vMsgId, uint16_t voltages[4]);

/**
 * @brief BMS Transmit Voltage Message Parser
 * @param packet The BMS_TransmitVoltage_t packet to parse
 * @param BMSId The BMS ID
 * @param vMsgId The voltage message ID (0 or 1) as we send 2 packets with all voltages
 * @param voltages The voltages parsed from the packet
 */
void Parse_BMS_TransmitVoltage(uint32_t canId, uint8_t* data, uint8_t* BMSId, uint8_t* vMsgId, uint16_t* voltages);

/**
 * @brief BMS Transmit Temperature Message
 */
typedef struct BMS_TransmitTemperature
{
	uint32_t id; /**< CAN Packet ID */
	uint8_t data[7]; /**< Data */
} BMS_TransmitTemperature_t;

/**
 * @brief BMS Transmit Temperature Message Composer
 * @param BMSId The BMS ID
 * @param tMsgId The temperature message ID (0 or 1) as we send 2 packets with all temperatures
 * @param temperatures THe temperatures to send
 * @return The composed BMS_TransmitTemperatues_t packet
 */
BMS_TransmitTemperature_t Compose_BMS_TransmitTemperature(uint8_t BMSId, uint8_t tMsgId, uint8_t temperatures[6]);

/**
 * @brief BMS Transmit Temperature Message Parser
 * @param packet The BMS_TransmitTemperature_t packet to parse
 * @param BMSId The BMS ID
 * @param tMsgId The temperature message ID (0 or 1) as we send 2 packets with all temperatures
 * @param temperatures The temperatures parsed from the packet
 */
void Parse_BMS_TransmitTemperature(uint32_t canId, uint8_t* data, uint8_t* BMSId, uint8_t* tMsgId, uint8_t* temperatures);

/**
 * @brief BMS Charge Enabled Message
 */
typedef struct BMS_ChargeEnabled
{
	uint32_t id; /**< CAN Packet ID */
} BMS_ChargeEnabled_t;

/**
 * @brief BMS Charge Enabled Message Composer
 * @param BMSId The BMS ID
 * @return The composed BMS_ChargeEnabled_t packet
 */
BMS_ChargeEnabled_t Compose_BMS_ChargeEnabled(uint8_t BMSId);

/**
 * @brief BMS Charge Enabled Message Parser
 * @param packet The BMS_ChargeEnabled_t packet to parse
 * @param BMSId The BMS ID
 */
void Parse_ChargeEnabled(uint32_t canId, uint8_t* BMSId);

#endif /* INC_BMS_CAN_MESSAGES_H_ */
