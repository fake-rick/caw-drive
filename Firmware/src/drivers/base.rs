pub trait BaseDriver {
    fn set_pwm(&mut self, ua: f32, ub: f32, uc: f32);
}
