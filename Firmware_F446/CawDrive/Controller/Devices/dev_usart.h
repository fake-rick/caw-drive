#ifndef __DEV_USART_H__
#define __DEV_USART_H__

#include <stdint.h>

int dev_usart_init(void);
int dev_usart_write(const uint8_t* buf, uint16_t len);

#endif