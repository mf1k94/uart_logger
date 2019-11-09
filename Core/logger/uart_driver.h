/*
 * uart_driver.h
 *
 *  Created on: Sep 16, 2019
 *      Author: Mati
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "usart.h"

void uart_driver_init();
uint8_t uart_put(const char* msg, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* UART_DRIVER_H_ */
