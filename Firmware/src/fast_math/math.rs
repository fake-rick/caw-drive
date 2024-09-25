use super::{
    defines::{_2PI, _PI_2},
    table::SIN_TABLE,
};

const MULTIPLIER: f32 = 81.4873308631;

pub fn fast_sin(mut theta: f32) -> f32 {
    while theta < 0.0 {
        theta += _2PI;
    }
    while theta >= _2PI {
        theta -= _2PI;
    }
    SIN_TABLE[(MULTIPLIER * theta) as usize]
}

pub fn fast_cos(theta: f32) -> f32 {
    fast_sin(_PI_2 - theta)
}

pub fn fast_sincos(theta: f32) -> (f32, f32) {
    (fast_sin(theta), fast_cos(theta))
}
