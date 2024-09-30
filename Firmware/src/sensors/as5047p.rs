#![allow(unused)]

use crate::{abs, even_check, fast_math::defines::_2PI, powtwo};
use defmt::{debug, error, info, warn};
use embassy_embedded_hal::shared_bus::asynch;
use embassy_time::{Instant, Timer};
use embedded_hal_async::spi::{self, Operation};

use super::base::BaseSensor;

const NOP: u16 = 0x0000;
const ERRFL: u16 = 0x0001;
const PROG: u16 = 0x0003;
const DIAAGC: u16 = 0x3ffc;
const MAG: u16 = 0x3ffd;
const ANGLEUNC: u16 = 0x3ffe;
const ANGLECOM: u16 = 0x3fff;
const OP_WRITE: u16 = 0x0000;
const OP_READ: u16 = 0x0001;
const BIT_RESOLUTION: u16 = 14;
const CPR: u16 = powtwo!(BIT_RESOLUTION);
const MIN_ELAPSED_TIME: f32 = 0.000100;

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
    angle_prev: f32,
    angle_prev_ts: u64,
    vel_angle_prev: f32,
    vel_angle_prev_ts: u64,
    velocity: f32,
    full_rotations: i64,
    vel_full_rotations: i64,
}

impl<SPI> AS5047P<SPI>
where
    SPI: spi::SpiDevice<u16>,
{
    pub fn new(spi: SPI) -> AS5047P<SPI> {
        AS5047P {
            spi,
            angle_prev: 0.0,
            angle_prev_ts: 0,
            vel_angle_prev: 0.0,
            vel_angle_prev_ts: 0,
            velocity: 0.0,
            full_rotations: 0,
            vel_full_rotations: 0,
        }
    }

    pub async fn init(&mut self) -> Result<(), &'static str> {
        let _ = self.get_sensor_angle().await?;
        Timer::after_micros(1).await;
        self.vel_angle_prev = self.get_sensor_angle().await?;
        self.vel_angle_prev_ts = Instant::now().as_micros();
        Timer::after_millis(1).await;
        let _ = self.get_sensor_angle().await?;
        Timer::after_micros(1).await;
        self.angle_prev = self.get_sensor_angle().await?;
        self.angle_prev_ts = Instant::now().as_micros();
        Ok(())
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

    pub async fn get_raw_count(&mut self) -> Result<u16, &'static str> {
        let _ = self.write(cmd_frame!(ANGLECOM, OP_READ)).await;
        let mut data = self.write(cmd_frame!(NOP, OP_READ)).await;
        if (data & (1u16 << 14)) > 0 {
            let _ = self.write(cmd_frame!(ERRFL, OP_READ)).await & 0x0003u16;
            return Err("read reg failed");
        }
        if data >> 15 != even_check!(data & 0x7fff) {
            return Err("even check failed");
        }
        Ok(data & 0x3fff)
    }

    pub async fn get_sensor_angle(&mut self) -> Result<f32, &'static str> {
        let val = self.get_raw_count().await?;
        Ok((val as f32 / CPR as f32) * _2PI)
    }
}

impl<SPI> BaseSensor for AS5047P<SPI>
where
    SPI: spi::SpiDevice<u16>,
{
    async fn update(&mut self) -> Result<(), &'static str> {
        let val = self.get_sensor_angle().await?;
        if val < 0.0 {
            return Err("angle < 0.0");
        }
        self.angle_prev_ts = Instant::now().as_micros();
        let d_angle = val - self.angle_prev;
        if abs!(d_angle) > (0.8 * _2PI) {
            if d_angle > 0.0 {
                self.full_rotations -= 1;
            } else {
                self.full_rotations += 1;
            }
        }
        self.angle_prev = val;
        Ok(())
    }

    async fn get_velocity(&mut self) -> f32 {
        let ts = (self.angle_prev_ts - self.vel_angle_prev_ts) as f32 * 1e-6;
        if ts < 0.0 {
            self.vel_angle_prev = self.angle_prev;
            self.vel_full_rotations = self.full_rotations;
            self.vel_angle_prev_ts = self.angle_prev_ts;
            return self.velocity;
        }
        if ts < MIN_ELAPSED_TIME {
            return self.velocity;
        }
        self.velocity = ((self.full_rotations - self.vel_full_rotations) as f32 * _2PI
            + (self.angle_prev - self.vel_angle_prev))
            / ts;

        self.vel_angle_prev = self.angle_prev;
        self.vel_full_rotations = self.full_rotations;
        self.vel_angle_prev_ts = self.angle_prev_ts;
        self.velocity
    }

    async fn get_angle(&mut self) -> f32 {
        self.full_rotations as f32 * _2PI + self.angle_prev
    }

    async fn get_full_rotations(&self) -> i64 {
        self.full_rotations
    }

    async fn get_mechanical_angle(&self) -> f32 {
        self.angle_prev
    }
}
