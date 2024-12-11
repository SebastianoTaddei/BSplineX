"""Create and evaluate a non-uniform B-Spline."""

import BSplineX as bs


def main() -> None:
    """Main function."""

    # initialize a cubic, non-uniform, open B-spline curve with given knots and control points
    a_degree = 3
    a_knots = [0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2]
    a_ctrl_points = [0.1, 1.3, 2.2, 4.9, 13.2]

    tmp = bs.open_nonuniform(a_degree, a_knots, a_ctrl_points)

    # evaluate the curve at some points
    eval_x = [3.0, 3.4, 5.1, 6.2]
    for val in eval_x:
        print(f"bspline.evaluate({val}) = {tmp.evaluate(val)}")


if __name__ == "__main__":
    main()
