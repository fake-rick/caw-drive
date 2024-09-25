use super::base::BaseDriver;

use embassy_stm32::{
    gpio::{Level, Output, OutputType, Speed},
    peripherals::TIM1,
    time::khz,
    timer::{
        low_level::CountingMode,
        simple_pwm::{PwmPin, SimplePwm},
        Channel,
    },
};

use crate::{constrain, PwmTimResources};

pub struct PWMX3 {
    pwm: SimplePwm<'static, TIM1>,
    pub voltage_power_supply: f32, // 电源电压
    pub voltage_limit: f32,        // 限制电压
    pub max_duty: f32,
    ch1n: Output<'static>,
    ch2n: Output<'static>,
    ch3n: Output<'static>,
}

impl PWMX3 {
    pub fn new(tim: PwmTimResources, voltage_power_supply: f32, voltage_limit: f32) -> Self {
        let ch1 = PwmPin::new_ch1(tim.tim1_ch1, OutputType::PushPull);
        let ch2 = PwmPin::new_ch2(tim.tim1_ch2, OutputType::PushPull);
        let ch3 = PwmPin::new_ch3(tim.tim1_ch3, OutputType::PushPull);

        let mut ch1n = Output::new(tim.tim1_ch1n, Level::High, Speed::Low);
        let mut ch2n = Output::new(tim.tim1_ch2n, Level::High, Speed::Low);
        let mut ch3n = Output::new(tim.tim1_ch3n, Level::High, Speed::Low);
        ch1n.set_high();
        ch2n.set_high();
        ch3n.set_high();

        let mut pwm = SimplePwm::new(
            tim.tim1,
            Some(ch1),
            Some(ch2),
            Some(ch3),
            None,
            khz(40),
            CountingMode::CenterAlignedBothInterrupts,
        );
        pwm.set_duty(Channel::Ch1, 0);
        pwm.set_duty(Channel::Ch2, 0);
        pwm.set_duty(Channel::Ch3, 0);

        pwm.enable(Channel::Ch1);
        pwm.enable(Channel::Ch2);
        pwm.enable(Channel::Ch3);

        let max_duty = pwm.get_max_duty() as f32;

        Self {
            pwm,
            voltage_power_supply,
            voltage_limit,
            max_duty,
            ch1n,
            ch2n,
            ch3n,
        }
    }
}

impl BaseDriver for PWMX3 {
    fn set_pwm(&mut self, mut ua: f32, mut ub: f32, mut uc: f32) {
        ua = constrain!(ua, 0.0, self.voltage_limit);
        ub = constrain!(ub, 0.0, self.voltage_limit);
        uc = constrain!(uc, 0.0, self.voltage_limit);

        let dc_a = constrain!(ua / self.voltage_power_supply, 0.0, 1.0);
        let dc_b = constrain!(ub / self.voltage_power_supply, 0.0, 1.0);
        let dc_c = constrain!(uc / self.voltage_power_supply, 0.0, 1.0);

        // debug!("dc_a:{:?} dc_b:{:?} dc_c:{:?}", dc_a, dc_b, dc_c);

        self.pwm
            .set_duty(Channel::Ch1, (dc_a * self.max_duty) as u32);
        self.pwm
            .set_duty(Channel::Ch2, (dc_b * self.max_duty) as u32);
        self.pwm
            .set_duty(Channel::Ch3, (dc_c * self.max_duty) as u32);
    }
}
