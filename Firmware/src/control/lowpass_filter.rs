use embassy_time::Instant;

pub struct LowPassFilter {
    tf: f32,
    timestamp_prev: u64,
    y_prev: f32,
}

impl LowPassFilter {
    pub fn new(tf: f32) -> Self {
        Self {
            tf,
            timestamp_prev: Instant::now().as_micros(),
            y_prev: 0.0,
        }
    }

    pub fn calc(&mut self, x: f32) -> f32 {
        let timestamp = Instant::now().as_micros();
        let mut dt = (timestamp - self.timestamp_prev) as f32 * 1e-6;
        if dt < 0.0 {
            dt = 1e-3;
        } else if dt > 0.3 {
            self.y_prev = x;
            self.timestamp_prev = timestamp;
            return x;
        }
        let alpha = self.tf / (self.tf + dt);
        let y = alpha * self.y_prev + (1.0 - alpha) * x;
        self.y_prev = y;
        self.timestamp_prev = timestamp;
        y
    }
}
