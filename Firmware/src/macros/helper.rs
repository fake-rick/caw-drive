#[macro_export]
macro_rules! constrain {
    ($amt:expr, $low:expr, $high:expr) => {{
        match ($amt < $low, $amt > $high) {
            (true, _) => $low,
            (_, true) => $high,
            _ => $amt,
        }
    }};
}

#[macro_export]
macro_rules! set_bit {
    ($val:expr, $i:expr) => {{
        ($val | (1 << $i))
    }};
}

#[macro_export]
macro_rules! clear_bit {
    ($val:expr, $i:expr) => {{
        ($val & (!(1 << $i)))
    }};
}

#[macro_export]
macro_rules! even_check {
    ($val:expr) => {{
        let mut val = $val;
        val ^= val >> 8;
        val ^= val >> 4;
        val ^= val >> 2;
        val ^= val >> 1;
        (val & 1)
    }};
}

#[macro_export]
macro_rules! powtwo {
    ($x:expr) => {{
        (1 << ($x))
    }};
}

#[macro_export]
macro_rules! abs {
    ($x:expr) => {{
        if $x >= 0.0 {
            $x
        } else {
            -$x
        }
    }};
}
