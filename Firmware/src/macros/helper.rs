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
