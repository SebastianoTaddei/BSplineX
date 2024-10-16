"""

Reference data for testing our implementation of the De Boor algorithm.

"""

from scipy.interpolate import BSpline
import numpy as np


def ref_open(knots: list, control_points: list, degree: int) -> tuple:
    """
    ref_open generates reference data for the open spline case.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    :return: x_values, y_values
    :rtype: tuple(list, list)
    """

    spline = BSpline(knots, control_points, degree)

    x_values = np.arange(knots[degree], knots[-degree], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def main():
    """
    main is the entry point of the reference module.
    """

    knots = [0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2]
    control_points = [0.1, 1.3, 2.2, 4.9, 13.2]
    degree = 3

    x_values, y_values = ref_open(knots, control_points, degree)

    print(f"std::vector<double> x_values{{{', '.join([str(x) for x in x_values])}}};")
    print(f"std::vector<double> y_values{{{', '.join([str(y) for y in y_values])}}};")


if __name__ == "__main__":
    main()
