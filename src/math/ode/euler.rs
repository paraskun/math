pub enum Scheme {
    FES,
    SES,
    TES,
}

pub struct Point(pub f64, pub f64);

pub fn solve(d: fn(f64, f64) -> f64, grid: &mut[Point], scheme: Scheme) {
    if grid.len() < 2 {
        return;
    }

    let step = grid[1].0 - grid[0].0;

    for i in 1..grid.len() {
        grid[i].1 = match scheme {
            Scheme::FES => grid[i - 1].1 + step * d(grid[i - 1].0, grid[i - 1].1),
            Scheme::SES => {
                grid[i - 1].1
                    + step / 2.0
                        * (d(grid[i - 1].0, grid[i - 1].1)
                            + d(
                                grid[i].0,
                                grid[i - 1].1 + step * d(grid[i - 1].0, grid[i - 1].1),
                            ))
            }
            Scheme::TES => {
                grid[i - 1].1
                    + step
                        * d(
                            grid[i / 2].0,
                            grid[i - 1].1 + step / 2.0 * d(grid[i - 1].0, grid[i - 1].1),
                        )
            }
        }
    }
}
