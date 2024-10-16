"""

Reference data for testing our implementation of the De Boor algorithm.

"""

from scipy.interpolate import BSpline
import numpy as np
import argparse


def parse_args():
    """
    parse_args parses the command line arguments.

    :return: the parsed arguments
    :rtype: argparse.Namespace
    """

    parser = argparse.ArgumentParser(
        description="Generate reference data for the De Boor algorithm."
    )
    parser.add_argument(
        "--knots", type=float, nargs="+", help="The knots vector.", required=True
    )
    parser.add_argument(
        "--control_points",
        type=float,
        nargs="+",
        help="The control points vector.",
        required=True,
    )
    parser.add_argument(
        "--degree", type=int, help="The degree of the spline.", required=True
    )
    parser.add_argument(
        "--type", type=str, help="The type of the spline.", required=True
    )
    parser.add_argument(
        "--plot", action="store_true", help="Whether to plot the reference data or not."
    )
    parser.add_argument(
        "--print",
        action="store_true",
        help="Whether to print the reference data or not.",
    )
    return parser.parse_args()


def check_args(knots: list, control_points: list, degree: int) -> None:
    """
    check_args checks the validity of the arguments.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    """

    if not isinstance(knots, list) and not isinstance(control_points, list):
        raise ValueError("The knots and control points must be lists.")
    if not isinstance(degree, int):
        raise ValueError("The degree must be an integer.")

    if len(control_points) != len(knots) - degree - 1:
        raise ValueError("len(control_points) != len(knots) - degree - 1.")


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

    spline = BSpline(knots, control_points, degree, extrapolate=False)

    x_values = np.arange(knots[degree], knots[-degree], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def ref_clamped(knots: list, control_points: list, degree: int) -> tuple:
    """
    ref_clamped generates reference data for the clamped spline case.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    :return: x_values, y_values
    :rtype: tuple(list, list)
    """

    knots = (
        [knots[0] for _ in range(degree)] + knots + [knots[-1] for _ in range(degree)]
    )

    spline = BSpline(knots, control_points, degree, extrapolate=False)

    x_values = np.arange(knots[degree], knots[-degree], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def main():
    """
    main is the entry point of the reference module.
    """

    args = parse_args()
    check_args(args.knots, args.control_points, args.degree)

    if args.type == "open":
        x_values, y_values = ref_open(args.knots, args.control_points, args.degree)
    elif args.type == "clamped":
        x_values, y_values = ref_clamped(args.knots, args.control_points, args.degree)
    else:
        raise NotImplementedError("The requested b-spline type is not implemented.")

    if args.plot:
        import matplotlib.pyplot as plt

        plt.figure()
        plt.plot(x_values, np.nan_to_num(y_values))
        plt.grid()
        plt.show()

    if args.print:
        print(
            f"std::vector<double> x_values{{{', '.join([str(x) for x in x_values])}}};"
        )
        print(
            f"std::vector<double> y_values{{{', '.join([str(y) for y in y_values])}}};"
        )


if __name__ == "__main__":
    main()
