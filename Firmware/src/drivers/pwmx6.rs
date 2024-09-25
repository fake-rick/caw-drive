use super::base::BaseDriver;
use defmt::debug;
use embassy_stm32::timer::complementary_pwm::{ComplementaryPwm, ComplementaryPwmPin};
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

pub struct PWMX6 {
    pwm: ComplementaryPwm<'static, TIM1>,
    pub voltage_power_supply: f32, // 电源电压
    pub voltage_limit: f32,        // 限制电压
    pub max_duty: f32,
}

impl PWMX6 {
    pub fn new(tim: PwmTimResources, voltage_power_supply: f32, voltage_limit: f32) -> Self {
        let ch1 = PwmPin::new_ch1(tim.tim1_ch1, OutputType::PushPull);
        let ch2 = PwmPin::new_ch2(tim.tim1_ch2, OutputType::PushPull);
        let ch3 = PwmPin::new_ch3(tim.tim1_ch3, OutputType::PushPull);

        let ch1n = ComplementaryPwmPin::new_ch1(tim.tim1_ch1n, OutputType::PushPull);
        let ch2n = ComplementaryPwmPin::new_ch2(tim.tim1_ch2n, OutputType::PushPull);
        let ch3n = ComplementaryPwmPin::new_ch3(tim.tim1_ch3n, OutputType::PushPull);

        let mut pwm = ComplementaryPwm::new(
            tim.tim1,
            Some(ch1),
            Some(ch1n),
            Some(ch2),
            Some(ch2n),
            Some(ch3),
            Some(ch3n),
            None,
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
        }
    }
}

impl BaseDriver for PWMX6 {
    fn set_pwm(&mut self, mut ua: f32, mut ub: f32, mut uc: f32) {
        ua = constrain!(ua, 0.0, self.voltage_limit);
        ub = constrain!(ub, 0.0, self.voltage_limit);
        uc = constrain!(uc, 0.0, self.voltage_limit);

        let dc_a = constrain!(ua / self.voltage_power_supply, 0.0, 1.0);
        let dc_b = constrain!(ub / self.voltage_power_supply, 0.0, 1.0);
        let dc_c = constrain!(uc / self.voltage_power_supply, 0.0, 1.0);

        self.pwm
            .set_duty(Channel::Ch1, (dc_a * self.max_duty) as u16);
        self.pwm
            .set_duty(Channel::Ch2, (dc_b * self.max_duty) as u16);
        self.pwm
            .set_duty(Channel::Ch3, (dc_c * self.max_duty) as u16);
    }
}
