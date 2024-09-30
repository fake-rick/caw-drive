#![allow(unused)]

use defmt::{debug, info};
use embassy_time::{Instant, Timer};

use crate::{
    abs,
    control::{lowpass_filter::LowPassFilter, pid::PIDController},
    drivers::{base::BaseDriver, pwmx3::PWMX3, pwmx6::PWMX6},
    fast_math::{
        defines::{_2PI, _3PI_2, _SQRT3_2},
        math::fast_sincos,
    },
    normalize_angle,
    sensors::base::BaseSensor,
};

pub struct DQVoltage {
    pub d: f32,
    pub q: f32,
}

impl DQVoltage {
    pub fn new() -> Self {
        Self { d: 0.0, q: 0.0 }
    }
}

pub struct DQCurrent {
    pub d: f32,
    pub q: f32,
}

impl DQCurrent {
    pub fn new() -> Self {
        Self { d: 0.0, q: 0.0 }
    }
}

pub enum ControlType {
    None,
    Velocity,
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
    shaft_velocity_sp: f32,
    shaft_angle: f32,
    shaft_angle_sp: f32,
    electrical_angle: f32,
    current_sp: f32,
    sensor: S,
    control_type: ControlType,
    pid_velocity: PIDController,
    lpf_velocity: LowPassFilter,
    voltage: DQVoltage,
    current: DQCurrent,
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
        pid_velocity: PIDController,
        lpf_velocity: LowPassFilter,
    ) -> Self {
        Self {
            pole_pairs,
            sensor_direction,
            driver,
            open_loop_timestamp: Instant::now().as_micros(),
            voltage_sensor_align: 3.0,
            zero_electric_angle: 0.0,
            shaft_velocity: 0.0,
            shaft_velocity_sp: 0.0,
            shaft_angle: 0.0,
            shaft_angle_sp: 0.0,
            electrical_angle: 0.0,
            current_sp: 0.0,
            sensor,
            control_type,
            pid_velocity,
            lpf_velocity,
            voltage: DQVoltage::new(),
            current: DQCurrent::new(),
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
        self.shaft_angle = normalize_angle!(self.shaft_angle + target * ts);
        self.shaft_velocity = target;
        let uq = self.driver.voltage_limit;
        self.set_phase_voltage(uq, 0.0, self.electrical_angle());
        self.open_loop_timestamp = now_us;
        uq
    }

    pub fn electrical_angle(&self) -> f32 {
        self.shaft_angle * self.pole_pairs as f32
    }

    pub async fn sensor_electrical_angle(&self) -> f32 {
        normalize_angle!(
            self.sensor_direction as f32
                * self.pole_pairs as f32
                * self.sensor.get_mechanical_angle().await
                - self.zero_electric_angle
        )
    }

    pub async fn shart_velocity(&mut self) -> f32 {
        self.sensor_direction as f32 * self.lpf_velocity.calc(self.sensor.get_velocity().await)
    }

    pub async fn step(&mut self, new_target: f32) -> Result<(), &'static str> {
        self.sensor.update().await?;
        self.shaft_velocity = self.shart_velocity().await;
        self.electrical_angle = self.sensor_electrical_angle().await;

        // if abs!(self.shaft_velocity) > 100.0 {
        //     debug!(
        //         "velocity: {:?} angle: {:?} full_rotations: {:?}",
        //         self.shaft_velocity,
        //         self.sensor.get_angle().await,
        //         self.sensor.get_full_rotations().await,
        //     );
        // }

        match self.control_type {
            ControlType::Velocity => {
                self.shaft_velocity_sp = new_target;
                self.current_sp = self
                    .pid_velocity
                    .calc(self.shaft_velocity_sp - self.shaft_velocity);
                self.voltage.q = self.current_sp;
                self.voltage.d = 0.0;
                self.set_phase_voltage(self.voltage.q, self.voltage.d, self.electrical_angle);
            }
            ControlType::VelocityOpenLoop => {
                self.shaft_velocity_sp = new_target;
                self.voltage.q = self.velocity_open_loop(self.shaft_velocity_sp);
                self.voltage.d = 0.0;
            }
            _ => (),
        }
        Ok(())
    }

    pub async fn align_sensor(&mut self) {
        self.set_phase_voltage(self.voltage_sensor_align, 0.0, _3PI_2);
        Timer::after_millis(700).await;
        self.sensor.update().await;
        self.zero_electric_angle = 0.0;
        self.zero_electric_angle = self.sensor_electrical_angle().await;
        Timer::after_millis(20);
        self.set_phase_voltage(0.0, 0.0, 0.0);
        Timer::after_millis(100).await;
    }
}
