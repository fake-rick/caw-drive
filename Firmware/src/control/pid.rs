#![allow(unused)]

use embassy_time::Instant;

use crate::constrain;
pub struct PIDController {
    p: f32,
    i: f32,
    d: f32,
    output_ramp: f32, // 输出值的变化率(V/S)
    limit: f32,       // 输出值限制(V)
    error_prev: f32,
    output_prev: f32,
    integral_prev: f32,
    timestamp_prev: u64,
}

impl PIDController {
    pub fn new(p: f32, i: f32, d: f32, ramp: f32, limit: f32) -> Self {
        Self {
            p,
            i,
            d,
            output_ramp: ramp,
            limit,
            error_prev: 0.0,
            output_prev: 0.0,
            integral_prev: 0.0,
            timestamp_prev: Instant::now().as_micros(),
        }
    }

    pub fn calc(&mut self, error: f32) -> f32 {
        let timestamp_now = Instant::now().as_micros();
        let mut ts = (timestamp_now - self.timestamp_prev) as f32 * 1e-6;
        if (ts <= 0.0 || ts > 0.5) {
            ts = 1e-3;
        }
        let proportional = self.p * error; // 计算比例
        let integral = constrain!(
            self.integral_prev + self.i * ts * 0.5 * (error + self.error_prev),
            -self.limit,
            self.limit
        ); // 计算积分
        let derivative = self.d * (error - self.error_prev) / ts;
        let mut output = constrain!(
            proportional + integral + derivative,
            -self.limit,
            self.limit
        );

        if self.output_ramp > 0.0 {
            let output_rate = (output - self.output_prev) / ts;
            if output_rate > self.output_ramp {
                output = self.output_prev + self.output_ramp * ts;
            } else if output_rate < -self.output_ramp {
                output = self.output_prev - self.output_ramp * ts;
            }
        }
        self.integral_prev = integral;
        self.output_prev = output;
        self.error_prev = error;
        self.timestamp_prev = timestamp_now;
        output
    }

    pub fn reset(&mut self) {
        self.integral_prev = 0.0;
        self.output_prev = 0.0;
        self.error_prev = 0.0;
    }
}
