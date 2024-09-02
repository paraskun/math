pub enum Scheme {
    FES,
    SES,
    TES,
}

pub struct Point(f64, f64);

pub fn solve(grid: &mut [Point], d: fn(f64, f64) -> f64, scheme: Scheme) {
    if grid.len() < 2 {
        return;
    }

    let step = grid[1].0 - grid[0].0;

    match scheme {
        Scheme::FES => {
            for i in 1..grid.len() {
                grid[i].1 = grid[i - 1].1 + step * d(grid[i - 1].0, grid[i - 1].1);
            }
        }
        Scheme::SES => {}
        Scheme::TES => {}
    }
}
