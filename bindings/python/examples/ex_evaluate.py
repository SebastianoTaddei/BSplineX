"""Create and evaluate a non-uniform B-Spline."""

import BSplineX as bs


def main() -> None:
    """Main function."""

    # initialize a cubic, non-uniform, open B-spline curve with given knots and control points
    degree = 3
    knots = [0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2]
    ctrl_points = [0.1, 1.3, 2.2, 4.9, 13.2]

    bspline = bs.open_nonuniform(degree, knots, ctrl_points)

    # evaluate the curve at some points
    eval_x = [3.0, 3.4, 5.1, 6.2]
    for val in eval_x:
        print(f"bspline.evaluate({val}) = {bspline.evaluate(val)}")


if __name__ == "__main__":
    main()
