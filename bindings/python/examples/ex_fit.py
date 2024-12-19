import BSplineX as bs
import matplotlib.pyplot as plt
import numpy as np


def main() -> None:
    """Main function."""

    # Points to fit
    x_fit = [2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9]
    y_fit = [11.2, 22.3, 13.4, 14.5, 25.6, 36.7, 17.8]

    # Initialize a cubic, uniform, periodic B-spline curve where knots are [0.1, 1.1, ..., 10.1, 11.1]
    degree = 3
    knots_begin = 0.1
    knots_end = 12.0
    num_knots = 12

    bspline = bs.periodic_uniform(degree, knots_begin, knots_end, num_knots)

    # Fit the curve to the points
    bspline.fit(x_fit, y_fit)

    # Evaluate the curve at some points. Since the curve is periodic, the evaluation can done at any point
    eval_x = np.linspace(-10, 20, 1000)
    eval_y = np.array([bspline.evaluate(x) for x in eval_x])

    plt.figure()
    plt.scatter(x_fit, y_fit)
    plt.plot(eval_x, eval_y)
    plt.show()


if __name__ == "__main__":
    main()
