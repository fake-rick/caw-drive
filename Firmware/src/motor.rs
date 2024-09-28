#![allow(unused)]

use defmt::debug;
use embassy_time::{Instant, Timer};

use crate::{
    drivers::{base::BaseDriver, pwmx3::PWMX3, pwmx6::PWMX6},
    fast_math::{
        defines::{_2PI, _3PI_2, _SQRT3_2},
        math::fast_sincos,
    },
    sensors::base::BaseSensor,
};
pub enum ControlType {
    None,
    VelocityOpenLoop,
}

pub struct Motor<S>
where
    S: BaseSensor,
{
    pole_pairs: u32,
    pub driver: PWMX3,
    open_loop_timestamp: u64,
    voltage_sensor_align: f32,
    zero_electric_angle: f32,
    sensor_direction: i32,
    shaft_velocity: f32,
    shaft_angle: f32,
    sensor: S,
    control_type: ControlType,
}

impl<S> Motor<S>
where
    S: BaseSensor,
{
    pub fn new(
        pole_pairs: u32,
        sensor_direction: i32,
        driver: PWMX3,
        sensor: S,
        control_type: ControlType,
    ) -> Self {
        Self {
            pole_pairs,
            sensor_direction,
            driver,
            open_loop_timestamp: Instant::now().as_micros(),
            voltage_sensor_align: 3.0,
            zero_electric_angle: 0.0,
            shaft_velocity: 0.0,
            shaft_angle: 0.0,
            sensor,
            control_type,
        }
    }

    fn set_phase_voltage(&mut self, uq: f32, ud: f32, angle_el: f32) {
        let (sa, ca) = fast_sincos(angle_el);
        // 反Park变换
        let ualpha = ca * ud - sa * uq;
        let ubeta = sa * ud + ca * uq;
        // Clarke变换
        let mut ua = ualpha;
        let mut ub = -0.5 * ualpha + _SQRT3_2 * ubeta;
        let mut uc = -0.5 * ualpha - _SQRT3_2 * ubeta;

        let mut center = self.driver.voltage_limit / 2.0;
        let umin = ua.min(ub.min(uc));
        let umax = ua.max(ub.max(uc));
        center -= (umax + umin) / 2.0;

        ua += center;
        ub += center;
        uc += center;

        self.driver.set_pwm(ua, ub, uc);
    }

    fn velocity_open_loop(&mut self, target: f32) -> f32 {
        let now_us: u64 = Instant::now().as_micros();
        let mut ts = (now_us - self.open_loop_timestamp) as f32 * 1e-6;
        if ts <= 0.0 || ts > 0.5 {
            ts = 1e-3;
        }
        self.shaft_angle = self.normalize_angle(self.shaft_angle + target * ts);
        self.shaft_velocity = target;
        let uq = self.driver.voltage_limit;
        self.set_phase_voltage(uq, 0.0, self.electrical_angle());
        self.open_loop_timestamp = now_us;

        uq
    }

    pub fn electrical_angle(&self) -> f32 {
        self.shaft_angle * self.pole_pairs as f32
    }

    pub fn normalize_angle(&self, angle: f32) -> f32 {
        let a = angle % _2PI;
        if a >= 0.0 {
            a
        } else {
            a + _2PI
        }
    }

    pub fn step(&mut self, new_target: f32) {
        match self.control_type {
            ControlType::VelocityOpenLoop => {
                self.velocity_open_loop(new_target);
            }
            _ => (),
        }
    }

    pub async fn align_sensor(&mut self) {
        self.set_phase_voltage(self.voltage_sensor_align, 0.0, _3PI_2);
        Timer::after_millis(700).await;
        self.set_phase_voltage(0.0, 0.0, 0.0);
        Timer::after_millis(100).await;
    }
}
