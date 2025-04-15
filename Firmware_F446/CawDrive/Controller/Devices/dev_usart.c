#include "./dev_usart.h"

#include "string.h"
#include "usart.h"

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t rx_buffer[RX_BUFFER_SIZE];

int dev_usart_init(void) {
  HAL_UART_Receive_DMA(&huart3, rx_buffer, RX_BUFFER_SIZE);
}

int dev_usart_write(const uint8_t* buf, uint16_t size) {
  if (size > TX_BUFFER_SIZE) {
    return -1;  // 错误：数据长度超过缓冲区限制
  }
  memcpy(tx_buffer, buf, size);
  return HAL_UART_Transmit_DMA(&huart3, tx_buffer, size);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size) {
  if (huart->Instance == USART3) {
  }
}

/// 下面错误处理函数中把奇偶校验错误以及数据溢出错误标志清除
/// 可以直接在调试时打断点查看huart->ErrorCode的值
void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
  if (huart->Instance == USART3) {
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_PEFLAG(huart);
    huart->RxState = HAL_UART_STATE_READY;
    HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buffer, RX_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
  }
}