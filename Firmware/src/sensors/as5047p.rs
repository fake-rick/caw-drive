use crate::even_check;
use defmt::error;
use embedded_hal_async::spi::{self, Operation};

const NOP: u16 = 0x0000;
const ERRFL: u16 = 0x0001;
const PROG: u16 = 0x0003;
const DIAAGC: u16 = 0x3ffc;
const MAG: u16 = 0x3ffd;
const ANGLEUNC: u16 = 0x3ffe;
const ANGLECOM: u16 = 0x3fff;
const OP_WRITE: u16 = 0x0000;
const OP_READ: u16 = 0x0001;
const ERROR: u16 = 0xffff;

macro_rules! cmd_frame {
    ($val:ident, $op:ident) => {{
        let mut val = $val;
        let op = $op;
        val |= (op << 14);
        val |= (even_check!(val) << 15);
        val
    }};
}

pub struct AS5047P<SPI> {
    spi: SPI,
}

impl<SPI> AS5047P<SPI>
where
    SPI: spi::SpiDevice<u16>,
{
    pub fn new(spi: SPI) -> AS5047P<SPI> {
        AS5047P { spi }
    }

    async fn write(&mut self, val: u16) -> u16 {
        let mut rx_data = [0u16; 1];
        if let Err(_) = self
            .spi
            .transaction(&mut [Operation::Transfer(&mut rx_data, &[val])])
            .await
        {
            error!("as5047p spi transfer failed");
        }
        rx_data[0]
    }

    pub async fn get_raw_data(&mut self) -> u16 {
        let _ = self.write(cmd_frame!(ANGLECOM, OP_READ)).await;
        let data = self.write(cmd_frame!(NOP, OP_READ)).await;
        if (data & (1u16 << 14)) > 0 {
            let _ = self.write(cmd_frame!(ERRFL, OP_READ)).await & 0x0003u16;
            return ERROR;
        }
        if data >> 15 != even_check!(data & 0x7fff) {
            return ERROR;
        }
        data & 0x7fff
    }
}
