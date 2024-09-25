use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::peripherals::*;
use embassy_stm32::{bind_interrupts, can};

use crate::resources::{Can2Resources, Can3Resources};

bind_interrupts!(pub struct Irqs {
    FDCAN2_IT0 => can::IT0InterruptHandler<FDCAN2>;
    FDCAN2_IT1 => can::IT1InterruptHandler<FDCAN2>;
    FDCAN3_IT0 => can::IT0InterruptHandler<FDCAN3>;
    FDCAN3_IT1 => can::IT1InterruptHandler<FDCAN3>;
});

#[embassy_executor::task]
pub async fn can2_task(_spawner: Spawner, r: Can2Resources) {
    let mut can2 = can::CanConfigurator::new(r.fdcan, r.rx_pin, r.tx_pin, Irqs);
    can2.properties().set_extended_filter(
        can::filter::ExtendedFilterSlot::_0,
        can::filter::ExtendedFilter::accept_all_into_fifo0(),
    );
    can2.properties().set_standard_filter(
        can::filter::StandardFilterSlot::_0,
        can::filter::StandardFilter::accept_all_into_fifo0(),
    );
    can2.set_bitrate(500_000);
    can2.set_fd_data_bitrate(5_000_000, false);
    let mut can2 = can2.start(can::OperatingMode::NormalOperationMode);

    loop {
        match can2.read_fd().await {
            Ok(envelope) => {
                let (_ts, rx_frame) = (envelope.ts, envelope.frame);
                info!(
                    "Rx: {} {:02x}",
                    rx_frame.header().len(),
                    rx_frame.data()[0..rx_frame.header().len() as usize],
                )
            }
            Err(err) => error!("Error in frame {:?}", err),
        }
    }
}

#[embassy_executor::task]
pub async fn can3_task(_spawner: Spawner, r: Can3Resources) {
    let mut can3 = can::CanConfigurator::new(r.fdcan, r.rx_pin, r.tx_pin, Irqs);
    can3.properties().set_extended_filter(
        can::filter::ExtendedFilterSlot::_0,
        can::filter::ExtendedFilter::accept_all_into_fifo0(),
    );
    can3.properties().set_standard_filter(
        can::filter::StandardFilterSlot::_0,
        can::filter::StandardFilter::accept_all_into_fifo0(),
    );
    can3.set_bitrate(500_000);
    can3.set_fd_data_bitrate(5_000_000, false);
    let mut can3 = can3.start(can::OperatingMode::NormalOperationMode);

    loop {
        match can3.read_fd().await {
            Ok(envelope) => {
                let (_ts, rx_frame) = (envelope.ts, envelope.frame);
                info!(
                    "Rx: {} {:02x}",
                    rx_frame.header().len(),
                    rx_frame.data()[0..rx_frame.header().len() as usize],
                )
            }
            Err(err) => error!("Error in frame {:?}", err),
        }
    }
}
