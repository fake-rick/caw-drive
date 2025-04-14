#include "./dev_usart.h"

#include <usart.h>

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t rx_buffer[RX_BUFFER_SIZE];

int dev_usart_init(void) {}

int dev_usart_write(const uint8_t* buf, uint16_t len) {}

/// 下面错误处理函数中把奇偶校验错误以及数据溢出错误标志清除
/// 可以直接在调试时打断点查看huart->ErrorCode的值
void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
  if (huart->Instance == USART6) {
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_PEFLAG(huart);
    huart->RxState = HAL_UART_STATE_READY;
    HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buffer, RX_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
  }
}