use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_time::Timer;

use crate::StateResources;

#[embassy_executor::task]
pub async fn check_state_task(_spawner: Spawner, r: StateResources) {
    let mut mcu_led = Output::new(r.mcu_sta_pin, Level::High, Speed::Low);
    let mut drv_led = Output::new(r.drv_sta_pin, Level::High, Speed::Low);

    loop {
        mcu_led.set_high();
        drv_led.set_high();
        Timer::after_millis(150).await;
        mcu_led.set_low();
        drv_led.set_low();
        Timer::after_millis(150).await;
    }
}
