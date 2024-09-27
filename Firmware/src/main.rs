#![no_std]
#![no_main]

mod comm;
mod drivers;
mod fast_math;
mod hws;
mod macros;
mod motor;
mod resources;
mod sensors;
mod tasks;

use crate::{hws::drv8323rs::*, Drv8323Resources};
use defmt::*;
use drivers::{pwmx3::PWMX3, pwmx6::PWMX6};
use embassy_embedded_hal::shared_bus::asynch::spi::SpiDevice;
use embassy_executor::Spawner;
use embassy_stm32::{
    gpio::{Level, Output, Speed},
    time::Hertz,
};
use embassy_time::Timer;
use hws::drv8323rs::DRV8232RS;
use motor::{ControlType, Motor};
use resources::*;
use sensors::as5047p::AS5047P;
use tasks::{
    can::{can2_task, can3_task},
    state::check_state_task,
    usart::usart1_task,
};
use {defmt_rtt as _, panic_probe as _};

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let mut config: embassy_stm32::Config = Default::default();
    {
        use embassy_stm32::rcc::*;
        // 配置RCC时钟树
        config.rcc.hsi = false;
        config.rcc.hse = Some(Hse {
            freq: Hertz(16_000_000),
            mode: HseMode::Oscillator,
        });
        config.rcc.pll = Some(Pll {
            source: PllSource::HSE,
            prediv: PllPreDiv::DIV4,
            mul: PllMul::MUL80,
            divp: Some(PllPDiv::DIV2),
            divq: Some(PllQDiv::DIV2), // FD CAN时钟
            divr: Some(PllRDiv::DIV2), // 系统时钟
        });
        config.rcc.mux.fdcansel = mux::Fdcansel::PLL1_Q;
        config.rcc.sys = Sysclk::PLL1_R;
        config.rcc.ahb_pre = AHBPrescaler::DIV1;
        config.rcc.apb1_pre = APBPrescaler::DIV1;
        config.rcc.apb2_pre = APBPrescaler::DIV1;
    }

    let p = embassy_stm32::init(config);
    let r = split_resources!(p);

    info!("[ CawFOC ]");

    // can bus configure
    let mut can_stb = Output::new(p.PD2, Level::High, Speed::High);
    can_stb.set_low();

    let spi3 = init_spi3(r.spi3).await;
    // drv8323 configure
    let drv_nss = Output::new(p.PA15, Level::High, Speed::Low);
    let drv_spi_dev = SpiDevice::new(spi3, drv_nss);

    // sensor configure
    let sensor_nss = Output::new(p.PA12, Level::High, Speed::Low);
    let sensor_spi_dev = SpiDevice::new(spi3, sensor_nss);
    let mut sensor = AS5047P::new(sensor_spi_dev);

    let mut drv = DRV8232RS::new(drv_spi_dev).await;
    Timer::after_millis(10).await;
    let mut enable = Output::new(r.drv8323.enable, Level::Low, Speed::Low);
    let mut cal = Output::new(r.drv8323.cal, Level::High, Speed::Low);
    Timer::after_millis(10).await;
    enable.set_high();
    cal.set_low();
    Timer::after_millis(10).await;
    drv.calibrate().await;
    Timer::after_millis(10).await;
    drv.write_dcr(0x0, DIS_GDF_DIS, 0x0, PWM_MODE_3X, 0x0, 0x0, 0x0, 0x0, 0x1)
        .await;
    Timer::after_millis(10).await;
    drv.write_csacr(0x0, 0x1, 0x0, CSA_GAIN_40, 0x0, 0x1, 0x1, 0x1, SEN_LVL_1_0)
        .await;
    Timer::after_millis(10).await;
    drv.write_csacr(0x0, 0x1, 0x0, CSA_GAIN_40, 0x1, 0x0, 0x0, 0x0, SEN_LVL_1_0)
        .await;
    Timer::after_millis(10).await;
    drv.write_ocpcr(
        TRETRY_50US,
        DEADTIME_50NS,
        OCP_NONE,
        OCP_DEG_8US,
        VDS_LVL_1_88,
    )
    .await;
    drv.dbg_reg_val().await;
    drv.enable_gd().await;
    Timer::after_millis(500).await;

    let mut motor = Motor::new(7, 1, PWMX3::new(r.pwm_tim, 12.0, 6.0), ControlType::None);

    spawner.spawn(can2_task(spawner, r.can2)).unwrap();
    spawner.spawn(can3_task(spawner, r.can3)).unwrap();
    spawner.spawn(usart1_task(spawner, r.usart1)).unwrap();
    spawner.spawn(check_state_task(spawner, r.state)).unwrap();

    loop {
        motor.step(5.0);
        let _ = sensor.get_raw_data().await;
        Timer::after_ticks(1).await;
    }
}
