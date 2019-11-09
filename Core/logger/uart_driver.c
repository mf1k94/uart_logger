/*
 * uart_driver.c
 *
 *  Created on: Sep 16, 2019
 *      Author: Mati
 */


#include "uart_driver.h"

#include "usart.h"
#include <string.h>
#include <stdbool.h>


/* Private defines */

#define MAX_MSG_LEN			40
#define MAX_TX_QUEUE_SIZE	16

/* ------------------------------------------------ */

/* Private structures definitions */

typedef struct {

	uint8_t buffer[MAX_MSG_LEN];
	uint8_t length;
} txMessage_t;

/* ------------------------------------------------ */

/* Private variables */

static txMessage_t txQueue[MAX_TX_QUEUE_SIZE];
static volatile uint8_t readIndex;
static volatile uint8_t writeIndex;
static volatile bool transsmisionInProgress;

/* ------------------------------------------------ */

/* Private function prototypes */

void incIndex(volatile uint8_t* index);
void addTxMessageToQueue(const char* message, uint8_t size);

/* ------------------------------------------------ */

void uart_driver_init(){

	for(uint8_t i = 0; i < MAX_TX_QUEUE_SIZE; i++){

		//reset all buffers
		memset(&txQueue[i], 0x00, sizeof(txMessage_t));
	}

	readIndex = 0;
	writeIndex = 0;
	transsmisionInProgress = false;
}

uint8_t uart_put(const char* msg, size_t size){

	uint8_t bytesSent = 0;
	if(size > MAX_MSG_LEN){

		bytesSent =  MAX_MSG_LEN;

		addTxMessageToQueue(msg, MAX_MSG_LEN);
	}
	else{

		bytesSent = size;
		addTxMessageToQueue(msg, size);
	}

	if(!transsmisionInProgress){

		if(HAL_UART_Transmit_DMA(&huart2, txQueue[readIndex].buffer, txQueue[readIndex].length) == HAL_OK){

			incIndex(&readIndex);
			transsmisionInProgress = true;
		}
	}

	return bytesSent;	// return number of bytes that was sent
}

/* ------------------------------------------------ */


/* Private function definitions */

void incIndex(volatile uint8_t* index){

	(*index)++;
	if(*index == MAX_TX_QUEUE_SIZE){

		*index = 0;
	}
}

void addTxMessageToQueue(const char* message, uint8_t size){

	memcpy(txQueue[writeIndex].buffer, message, size);
	txQueue[writeIndex].length = size;

	incIndex(&writeIndex);
}

/* ------------------------------------------------ */


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

	if(writeIndex != readIndex){

		if(HAL_UART_Transmit_DMA(huart, txQueue[readIndex].buffer, txQueue[readIndex].length) != HAL_OK){

			transsmisionInProgress = false;
		}
		else{

			incIndex(&readIndex);
		}
	}
	else{

		transsmisionInProgress = false;
	}
}
