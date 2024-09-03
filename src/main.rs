use algo::math::ode::euler;

fn derivative(t: f64, y: f64) -> f64 {
    2.0 * t * y
}

fn main() {
    let step = 0.5;

    let mut grid = Vec::<euler::Point>::new();
    let mut t = 0.0;

    grid.push(euler::Point(t, 1.0));

    for _i in 1..10 {
        t += step;
        grid.push(euler::Point(t, 0.0));
    }

    euler::solve(derivative, &mut grid[..], euler::Scheme::FES)
}
