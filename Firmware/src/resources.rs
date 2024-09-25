use crate::assign_resources;
use embassy_stm32::{mode::Async, peripherals, spi, time::Hertz};
use embassy_sync::{blocking_mutex::raw::NoopRawMutex, mutex::Mutex};
use static_cell::StaticCell;

assign_resources! {
    state: StateResources {
        drv_sta_pin: PB1,
        mcu_sta_pin: PB2,
    },
    can2: Can2Resources {
        fdcan: FDCAN2,
        rx_pin: PB5,
        tx_pin: PB6,
    },
    can3: Can3Resources {
        fdcan: FDCAN3,
        rx_pin: PB3,
        tx_pin: PB4,
    },
    usart1: Usart1Resources {
        usart: USART1,
        rx_pin: PC5,
        tx_pin: PC4,
        dma1_ch3: DMA1_CH3,
        dma1_ch4: DMA1_CH4,
    },
    spi3: Spi3Resources {
        spi: SPI3,
        mosi: PC12,
        miso: PC11,
        sck: PC10,
        dma1_ch1: DMA1_CH1,
        dma1_ch2: DMA1_CH2,
    },
    pwm_tim: PwmTimResources {
        tim1: TIM1,
        tim1_ch1: PA8,
        tim1_ch2:PA9,
        tim1_ch3:PA10,
        tim1_ch1n:PB13,
        tim1_ch2n:PB14,
        tim1_ch3n:PB15,
    },
    drv8323: Drv8323Resources {
        cal: PC7,
        enable: PC8,
        fault: PC9,
    }
}

/// Initialize SPI3 BUS
/// Supports multi-task sharing
pub type Spi3Bus = Mutex<NoopRawMutex, spi::Spi<'static, Async>>;

pub async fn init_spi3(r: Spi3Resources) -> &'static Spi3Bus {
    let mut config = spi::Config::default();
    config.frequency = Hertz(2_000_000);
    config.bit_order = spi::BitOrder::MsbFirst;
    config.mode = spi::MODE_1;
    let spi = spi::Spi::new(r.spi, r.sck, r.mosi, r.miso, r.dma1_ch1, r.dma1_ch2, config);
    static SPI_BUS: StaticCell<Spi3Bus> = StaticCell::new();
    SPI_BUS.init(Mutex::new(spi))
}
