use defmt::info;
use embassy_executor::Spawner;

use embassy_stm32::{
    bind_interrupts,
    mode::Async,
    peripherals,
    usart::{self, Config, UartTx},
};

use crate::Usart1Resources;

use super::messages::{Commands, USART_WRITE_SIGNAL};

bind_interrupts!(struct Irqs {
    USART1 => usart::InterruptHandler<peripherals::USART1>;
});

#[embassy_executor::task]
pub async fn usart1_write_task(mut tx: UartTx<'static, Async>) {
    loop {
        let cmd = USART_WRITE_SIGNAL.wait().await;
        match cmd {
            Commands::UsartTxBytes(buf) => {
                tx.write(buf).await.unwrap();
            }
            Commands::UsartTxStr(str) => {
                info!("{:?}", str);
                tx.write(str.as_bytes()).await.unwrap();
            }
        }
    }
}

#[embassy_executor::task]
pub async fn usart1_task(spawner: Spawner, r: Usart1Resources) {
    let mut config = Config::default();
    config.baudrate = 921600;
    let usart = usart::Uart::new(
        r.usart, r.rx_pin, r.tx_pin, Irqs, r.dma1_ch3, r.dma1_ch4, config,
    )
    .unwrap();
    let (tx, mut rx) = usart.split();
    spawner.spawn(usart1_write_task(tx)).unwrap();
    loop {
        let mut buf = [0; 1];
        rx.read(&mut buf[..]).await.unwrap();
    }
}
